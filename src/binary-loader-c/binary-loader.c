#include <stdio.h>
#include <stdlib.h>
#include "bstrlib.h"

int main() {
    char *line = NULL;
    size_t size;
    int i=0;
    while(getline(&line, &size, stdin) != -1) {
        bstring bline = bfromcstr(line);

        printf("%d:%s", i++, line);

        bdestroy(bline);
    }
    free(line);
    return 0;
}