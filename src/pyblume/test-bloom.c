#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>

#include "blume.h"

#define N (1024 * 1024 * 1024)

void printErr(blume_error err);

int main(int argc, char *argv[])
{
    struct blume *bf;
    int done;
    uint64_t i;
    blume_error err;

    mtrace();

    // err = blume_create(&bf, 1024 * 1024 * 19, 0.1, "/tmp/test.bf");

    // for(i = 0; i < N; i++) {
    //     if ((i % 10) == 0) {
    //         err = blume_add(bf, &i, sizeof(i));
    //         if (err != BLUME_ERROR_SUCCESS) {
    //             fprintf(stderr, "WTF!\n");
    //         }
    //     }
    // }

    // for(i = 0; i < N; i++) {
    //     count_total += 1;
    //     int hit = blume_check(bf, &i, sizeof(i));
    //     if (hit && ((i % 10) != 0)) {
    //         count_fps += 1;
    //     } else if (!hit && ((i % 10) == 0)) {
    //         count_fns += 1;
    //     }
    // }


    // printf("total: %lu\nfps: %lu\nfns: %lu\n", count_total, count_fps, count_fns);
    // printf("P(fp): %0.6f\n", ((double)count_fps / count_total));

    // blume_close(&bf);


    // err = blume_open(&bf, "/tmp/test.bf", 0);
    // if (err != BLUME_ERROR_SUCCESS) {
    //     fprintf(stderr, "WTF!\n");
    // }

    // count_fps = 0;
    // count_fns = 0;
    // count_total = 0;
    // for(i = 0; i < N; i++) {
    //     count_total += 1;
    //     int hit = blume_check(bf, &i, sizeof(i));
    //     if (hit && ((i % 10) != 0)) {
    //         count_fps += 1;
    //     } else if (!hit && ((i % 10) == 0)) {
    //         count_fns += 1;
    //     }
    // }


    // printf("total: %lu\nfps: %lu\nfns: %lu\n", count_total, count_fps, count_fns);
    // printf("P(fp): %0.6f\n", ((double)count_fps / count_total));

    // blume_close(&bf);

    srand(0);
    i = 0;
    done = 0;
    err = blume_create(&bf, 1024 * 1024 * 1024, 0.01, "./scale.bf");
    while(!done) {
        i += 1;
        char c[32];
        sprintf(c, "%08X%08X", rand(),rand());
        err = blume_add(bf, c, 16);
        if (err != BLUME_ERROR_SUCCESS) {
            printErr(err);
            fprintf(stderr, "Failed at %lu entries.\n", i);
            done = 1;
        }
    }

    int s = i;

    uint64_t count_fps = 0;
    uint64_t count_fns = 0;
    uint64_t count_total = 0;

    for(i = s; i < s*2; i++) {
        count_total += 1;
        char c[32];
        sprintf(c, "%08X%08X", rand(),rand());
        int hit = blume_check(bf, c, 16);
        if(hit) count_fps += 1;
    }

    printf("total: %lu\nfps: %lu\nfns: %lu\n", count_total, count_fps, count_fns);
    printf("P(fp): %0.6f\n", ((double)count_fps / count_total));

    blume_close(&bf);

    muntrace();

    return 0;
}

void printErr(blume_error err) {
    switch(err) {
        case BLUME_ERROR_SUCCESS:        printf("%s\n", "BLUME_ERROR_SUCCESS");       break;
        case BLUME_ERROR_ERRNO:          printf("%s\n", "BLUME_ERROR_ERRNO");         break;
        case BLUME_ERROR_OUT_OF_MEMORY:  printf("%s\n", "BLUME_ERROR_OUT_OF_MEMORY"); break;
        case BLUME_ERROR_OUT_OF_FILTERS: printf("%s\n", "BLUME_ERROR_OUT_OF_FILTERS");break;
        case BLUME_ERROR_FULL:           printf("%s\n", "BLUME_ERROR_FULL");          break;
        case BLUME_ERROR_LOCKED:         printf("%s\n", "BLUME_ERROR_LOCKED");        break;
        case BLUME_ERROR_CORRUPT:        printf("%s\n", "BLUME_ERROR_CORRUPT");       break;
    }
}
