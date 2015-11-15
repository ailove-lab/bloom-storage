/* Copyright (c) 2012-2014 SEOmoz
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* This file is used as a driver for debugging purposes */

#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "bloom.h"

int main(int argc, char ** argv) {
    pyrebloomctxt ctxt;
    uint32_t i;
    time_t start, end;
    uint32_t count = 10000000;

    init_pyrebloom(&ctxt, "bloom", count, 0.01, "localhost", 6379, "", 0);
    int L=0;
    
    srand(0);
    time(&start);
    for (i = 0; i < count; ++i) {
        int r = rand();
        char c[32];
        sprintf(c, "%08X", r);
        // printf("%s", c);
        L += strlen(c);
        add(&ctxt, c, strlen(c));
    }
    add_complete(&ctxt, count);
    time(&end);
    printf("ADD %f\n", difftime(end,start));
    
    // printf("\n");
    // printf("----------------------------\n");
    // printf("size: %d\n", L);
    // printf("----------------------------\n");

    // srand(0);
    time(&start);
    for (i = 0; i < count; ++i) {
        int r = rand();
        // rand();
        char c[32];
        sprintf(c, "%08X", r);
        // printf("%s", c);
        check(&ctxt, c, strlen(c));
    }

    int cnt = 0;
    for (i = 0; i < count; ++i) {
        int res = check_next(&ctxt);
        if(res) cnt++; 
    }
    time(&end);
    printf("CHECK %f\n", difftime(end,start));

    printf("count: %d false positive:%d prob: %f %%\n", count, cnt, 100.0/count*cnt);
    delete(&ctxt);
    free_pyrebloom(&ctxt);

    // time(&start);
    // for (i = 0; i < count; ++i) {
    //     redisAppendCommand(ctxt.ctxt, "SADD testing2 hello");
    // }
    // add_complete(&ctxt, count / 4);
    // time(&end);
    // printf("sadd      : %f\n", difftime(end, start));

    return 0;
}
