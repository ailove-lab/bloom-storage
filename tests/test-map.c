#include <stdio.h>
#include <stdlib.h>
#include "khash.h"

KHASH_MAP_INIT_STR(str, char*)

int
main(void)
{
   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   khash_t(str) *h = kh_init(str);

   fp = fopen("test-data.txt", "r");
   if (fp == NULL)
       exit(EXIT_FAILURE);

   while ((read = getline(&line, &len, fp)) != -1) {
       // printf("Retrieved line of length %zu :\n", read);
       // printf("%s", line);
       char *val = strchr(line,'\t');
       if(val == NULL) continue;
       *val = 0;
       val++;
       int ret = 0;
       int k = kh_put(str, h, line, &ret);
       // printf("%d\n", ret);
       kh_value(h, k) = val;
       line = NULL;
   }

   fclose(fp);
   
   for (int k = kh_begin(h); k != kh_end(h); ++k) {
        if (kh_exist(h,k)) {
            char *key = (char*) kh_key(h,k);
            free(key);
        }
   }

   kh_destroy(str,h);
   exit(EXIT_SUCCESS);
}