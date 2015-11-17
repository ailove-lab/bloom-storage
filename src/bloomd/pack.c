
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

int nthr;

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
// WARNING p must be less than number ofstrings x3!

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
        while ( s < E && s[-1] != '\n') s++;
    
    if (n<p-1)
        while ( e < E && e[0] != '\n') e++;

    if (s == e) return;
    sub->start  = s;
    sub->length = (n<p-1) ? e-s : E-s;

}

static void parser(void *blk, long i, int tid) {
    printf("[%02d]: %06ld\n", tid, i);
    sleep(1);
}
// n_threads - number of threads
// function (data, call number, thread id)
// data
// number of calls
void kt_for(int n_threads, void (*func)(void*, long, int), void *data, long n);

int main(int argc, char *argv[]) {

    if(argc < 2) return usage();

    nthr = sysconf(_SC_NPROCESSORS_ONLN);

    printf("Processors: %d\n", nthr);
    block b = {NULL, 0};

    loadFile(argv[1], &b);
    assert(b.start != NULL);
    
    // block s = {NULL, 0};
    // getSubBlock(&b, &s, i, bc);
    kt_for(nthr, parser, &b, nthr*10);

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
