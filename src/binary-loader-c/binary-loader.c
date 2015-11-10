#include <stdio.h>
#include "bstrlib.h"

int main() {
    char *line = NULL;
    size_t size;
    int i=0;
    while(getline(&line, &size, stdin) > 0) {
        printf("%d:%s", i++, line);
    }
    return 0;
}