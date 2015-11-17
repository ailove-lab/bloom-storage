
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <libmill.h>

typedef struct {
    char *start;
    unsigned long length;
} block;

void loadFile(char *filename, block *b) {

    FILE *infile;
     
    infile = fopen(filename, "r");
    if(infile == NULL) return;
     
    fseek(infile, 0L, SEEK_END);
    b->length = ftell(infile);
     
    fseek(infile, 0L, SEEK_SET);    
     
    b->start = (char*)calloc(b->length, sizeof(char)); 
    if(b->start == NULL) return;
     
    fread(b->start, sizeof(char), b->length, infile);
    fclose(infile);

}

static int usage(){ printf("usage: pack file\n"); return 1;}

// split block on 'p' parts
// get part n
// search first \n for start
// search last  \n for length
//                          
// #######n #######n #######n
//                           ^
//                           E
void getSubBlock(block *blk, 
                 block *sub, 
                 unsigned int n, 
                 unsigned int p) {

    if(n>p-1 || p > 128) return;

    char *E = blk->start + blk->length;
    unsigned int l = blk->length / p;
    char *s = blk->start + n*l;
    char *e = s + l;

    if (n>0)
        while ( s < E && *s != '\n') s++;
    
    if (n<p-1)
        while ( e < E && *e != '\n') e++;

    if (s == e) return;
    printf("%X %X\n", s, e);
    sub->start  = s;
    sub->length = e-s;

}

void parser(char* buf, unsigned long size) {

}

int main(int argc, char *argv[]) {

    if(argc == 1) return usage();

    block b = {NULL, 0};

    loadFile(argv[1], &b);
    if(b.start == NULL) return usage();
    
    block s = {NULL, 0};
    printf("blk: %X [%lu]\n", b.start, b.length);
    getSubBlock(&b, &s, 1, 10);
    printf("sub: %X [%lu]\n", s.start, s.length);
    assert(s.start != NULL);
    printf("%.*s\n", (int)s.length, s.start);

    free(b.start);

    return 0;
    // char buf[BUF_SIZE];

    // ipaddr addr = ipremote("localhost", 8673, 0, -1);
    // tcpsock s = tcpconnect(addr, -1);

    // if (s==NULL) return -1;
    
    // size_t nbytes = tcpsend(s,"list\n", 5, -1);
    // tcpflush(s,-1);
    // int i = 0;

    // do {
    //     nbytes = tcprecvuntil(s, buf, BUF_SIZE, "\n", 1, -1);
    //     printf("%d[%zu]: %.*s", i++, nbytes, (int)nbytes, buf);
    // } while (strncmp(buf,"END\n", nbytes)!=0);

    // tcpclose(s);

}
