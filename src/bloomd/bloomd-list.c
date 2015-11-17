
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <libmill.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    char buf[BUF_SIZE];

    ipaddr addr = ipremote("localhost", 8673, 0, -1);
    tcpsock s = tcpconnect(addr, -1);

    if (s==NULL) return -1;
    
    size_t nbytes = tcpsend(s,"list\n", 5, -1);
    tcpflush(s,-1);
    int i = 0;

    do {
        nbytes = tcprecvuntil(s, buf, BUF_SIZE, "\n", 1, -1);
        printf("%d[%zu]: %.*s", i++, nbytes, (int)nbytes, buf);
    } while (strncmp(buf,"END\n", nbytes)!=0);

    tcpclose(s);


    return 0;
}

