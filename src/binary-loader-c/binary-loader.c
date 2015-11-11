
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "hash.h"

int main() {

    char *line = NULL;
    size_t size;

    int t = -1;
    int s = -1;
    int e = -1;

    hash_t *index = hash_new();
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    while(getline(&line, &size, stdin) != -1) {
   
        // bstring bline = bfromcstr(line);
        
        // get key and val start   
        for (uint i=0; i<size; i++) {
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
            continue;
        } 

        char *key = strndup(&line[0],t);
        int vln = e-s;
        char *val = strndup(&line[s],vln);

        hash_set(index, key, val);
        // printf("key: \"%s\" val: \"%s\"\n", key, val);
    }
    free(line);
    printf("Duration %.2f sec\n", (double)(stop.tv_usec - start.tv_usec)*1e-6);

    // fprintf(stderr,"Duration: %.2f\n", (double)(time(NULL) - start));

    fprintf(stderr, "Hash values: %d\n", hash_size(index));
    fprintf(stderr, "hashed, wait 5 sec\n");
    sleep(5);

    fprintf(stderr, "Clear\n");
    hash_each(index, {
      // printf("%s: %s\n", key, (char *) val);
      free((char *)key);
      free((char *)val);
    });
    hash_free(index);
    fprintf(stderr, "Cleared, wait 5 sec\n");
    sleep(5);

    // const char *key; 
    // void *val; 

    // for (khiter_t k = kh_begin(index); k < kh_end(index); ++k) { 
    //   if (!kh_exist(index, k)) continue; 
    //   key = kh_key(index, k); 
    //   val = kh_value(index, k); 
    // } 


    return 0;
}