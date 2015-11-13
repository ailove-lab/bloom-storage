#ifndef BINLOAD
#define BINLOAD

#define KEY_LENGTH 16
#define MAX_SEGS_PER_ENTRY 100

#include "khash.h"

// Инициализиурем структуру для индекса сегментов
KHASH_MAP_INIT_INT(int2short, unsigned short)

khash_t(int2short) *seg_index;
void initSegIndex();
unsigned short segIndexation(int seg, int *ret);
void clearSegIndex();

// Инициализиурем структуру для счетчика ключей
KHASH_MAP_INIT_INT(int2int, int)

khash_t(int2int) *seg_counter;
void initSegCounter();
unsigned short segCount(int seg, int *ret);
void printSegCounter();
void clearSegCounter();

typedef struct {
    unsigned char key[KEY_LENGTH];
    unsigned char len;
    unsigned short *val;
} entry;

void parseEntry(const char *line, size_t size, char **key, char **val);
void decodeKey(char *key);
void decodeVal(char *val);



#endif