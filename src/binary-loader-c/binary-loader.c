
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary-loader.h"
#include "hash.h"
#include "base64.h"

/* 

    Структуры

        entry

            зпись имеет следуюший формат
            [base64 key]\t[flags] [segment_1/segment_2/.../segment_n]

        compressed entry
            [key 16 byte][seg_count 1 byte][seg 2 * seg_count] ...

        key 
            
            base64 string, ключ записи   
        
        index

            Индекс сегментов записи
            map[segment] -> char[2]
        
Алгоритм:
    
    Читаем построчно записи
    Декодируем base64 в unsigned char массив (22 байта -> 16 байт)
    Извлекаем запись, декодируем в unsigned int массив сегментов
    Индексируем сегменты, сторим map seg->code
    Собираем статистику по сегментам code->count

*/

int main() {

    char *line = NULL;
    size_t size;
    
    initSegIndex();
    initSegCounter();

    while(getline(&line, &size, stdin) != -1) {
   
        char *key = NULL;
        char *val = NULL;
        parseEntry(line, size, &key, &val);   
        if(key==NULL) continue;
        decodeKey(key);
        decodeVal(val);   
        
        // hash_set(index, key, val);
    }
    // readline - mallocs space
    free(line);

    // fprintf(stderr, "Hash values: %d\n", hash_size(index));
    // hash_each(index, {
    //   printf("%s: %s\n", key, (char *) val);

    //   // release key/val - parseEntry strdup key/val on heap
    //   free((char *)key);
    //   free((char *)val);
    // });
    // hash_free(index);
    
    printSegCounter();

    clearSegCounter();
    clearSegIndex();

    return 0;
}


void parseEntry(const char *line, size_t size, char **key, char **val) {
    
    int t = -1;
    int s = -1;
    int e = -1;

    // get key and val start   
    for (int i=0; i<size; i++) {
        if(line[i]=='\t') t = i;
        if(line[i]==' ') s = i+1;
        if(line[i]=='\n' || line[i]=='\r') {
            e = i;
            break;
        }
    }
    // don't have \t or \s
    if(s<0 || t<0 || e<0) {
        fprintf(stderr, "wrong format: %s\n", line);
        return;
    } 

    *key = strndup(&line[0],t);

    int vln = e-s;
    *val = strndup(&line[s],vln);
}

// decode base 64
void decodeKey(char *key) {
    char d[100];
    size_t kl = strlen(key);
    // const char *src = "This string gets encoded";
    int dl = base64_decode(d, sizeof(d), key, kl);
    // printf("%s [%zu->%zd]: ", key, kl, dl);
    // printf("Returned data of length %zd @%p ", dl, &d);
    // printf("key: ");
    // for(int i=0; i<dl; ++i) {
    //     printf("%02X", (unsigned char) d[i]);
    // }
    // printf("\n");
    // base64_decode mallocs mem for result
    // free(d);
}

void decodeVal(char *val) {
    int val_length = strlen(val);
    char *segs[MAX_SEGS_PER_ENTRY];
    unsigned int seg_count = 0;
    segs[seg_count++] = val; // first segment

    // search all '/'
    for(int i=0; i<val_length; i++) {
        if(val[i]!='/') continue;
        segs[seg_count++] = &val[i+1];
    }

    for(int i=0; i<seg_count; i++) {
        int seg = atoi(segs[i]);
        int ret=-1;
        //printf("seg: %08X ", seg);
        
        unsigned short id = segIndexation(seg, &ret);
        //printf("ind: %04X (%d) ", id, ret);

        int cnt = segCount(id, &ret);
        //printf("cnt: %d (%d)\n", cnt, ret);
    }

    //printf("\n");
}


// Segment index
// map int -> short

void initSegIndex() {
    // allocate hash table
    seg_index = kh_init(int2short);
}

unsigned short segIndexation(int seg, int *ret) {
    static unsigned short last_index = 1;
    
    khint_t k = kh_put(int2short, seg_index, seg, ret);
    
    switch(*ret) {
        case -1: // Failed :(
            return 0;
        case  0: // Key already present
            return kh_value(seg_index, k);
        case  1: // New key
            return kh_value(seg_index, k) = last_index++;
        case  2: // Deleted
            return 0;
        default:
            return 0;
    }
}

void clearSegIndex() {
    // clear memory
    kh_destroy(int2short, seg_index);
}

// Segment counter
// map short code -> counter 

void initSegCounter() {
    // allocate hash table
    seg_counter = kh_init(int2int);
}

unsigned short segCount(int seg, int *ret) {
    
    khint_t k = kh_put(int2int, seg_counter, seg, ret);
    
    switch(*ret) {
        case -1: // Failed :(
            return 0;
        case  0: // Key already present
            return kh_value(seg_counter, k)++;
        case  1: // New key
            return kh_value(seg_counter, k) = 1;
        case  2: // Deleted
            return 0;
        default:
            return 0;
    }
}

void printSegCounter() {
    for (khint_t k = kh_begin(seg_counter); k != kh_end(seg_counter); ++k)  // traverse
        if (kh_exist(seg_counter, k))                             // test if a bucket contains data
            printf("%04X %*d |%s", kh_key(seg_counter, k), 6, kh_value(seg_counter, k), !(k%16) ? "\n": " ");
}
void clearSegCounter() {
    // clear memory
    kh_destroy(int2int, seg_counter);
}