#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int segments[9];
    for (int i=0; i<9;i++) segments[i] = (i+1)*100000+i+1; 

    int cnt = argc>1 ? atoi(argv[1]) : 1000;
    unsigned long k=0;    
    for(int i=1; i<=cnt; i++) {
        printf("%022d\t", i);
        for (int j=0; j<8; j++) printf("%c ", k++%3 ? '-' :'A'+j);
        for (int m=0; m<8; m++,k++) printf("%d%c", segments[k%9], m==7 ? '\n':'/'); 
    }
    return 0;
}