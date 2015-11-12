
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "base64.h"

/* 

    Структуры

        entry

            зпись имеет следуюший формат
            [base64 key]\t[flags] [segment_1/segment_2/.../segment_n]

        key 
            
            base64 string, ключ записи   
        
        index

            Индекс сегментов записи
            map[segment] -> char[2]


*/

#define KEY_LENGTH 16

typedef struct {
    unsigned char key[KEY_LENGTH];
    unsigned char len;
    unsigned short *val;
} entry;

void parseEntry(const char *line, size_t size, char **key, char **val);
void decodeKey(char *key);
void decodeVal(char *val);

int main() {

    char *line = NULL;
    size_t size;
    
    hash_t *index = hash_new();
    
    while(getline(&line, &size, stdin) != -1) {
   
        char *key = NULL;
        char *val = NULL;
        parseEntry(line, size, &key, &val);   
        if(key==NULL) continue;
        decodeKey(key);
        decodeVal(val);   
        
        hash_set(index, key, val);
    }
    // readline - mallocs space
    free(line);

    fprintf(stderr, "Hash values: %d\n", hash_size(index));
    hash_each(index, {
      printf("%s: %s\n", key, (char *) val);

      // release key/val - parseEntry strdup key/val on heap
      free((char *)key);
      free((char *)val);
    });
    hash_free(index);

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

void decodeKey(char *key) {
    char d[100];
    size_t kl = strlen(key);
    // const char *src = "This string gets encoded";
    int dl = base64_decode(d, sizeof(d), key, kl);
    // printf("%s [%zu->%zd]: ", key, kl, dl);
    // printf("Returned data of length %zd @%p ", dl, &d);
    printf("key: ");
    for(int i=0; i<dl; ++i) {
        printf("%02X", (unsigned char) d[i]);
    }
    // printf("\n");
    // base64_decode mallocs mem for result
    // free(d);
}

void decodeVal(char *val) {
    int l = strlen(val);
    int seg = atoi(val);
    printf(" val: ");
    printf("%d ", seg);
    for(int i=0; i<l; i++) {
        if(val[i]!='/') continue;
        seg = atoi(&val[i+1]);
        printf("%d ", seg);
    }
    printf("\n");
}