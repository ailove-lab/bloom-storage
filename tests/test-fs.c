#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE (1 * 1024 * 1024)
#define ITERATIONS (10 * 1024)

char* file_name = "../data/part-00001";

double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.;
}

int main()
{
    unsigned char buffer[BUFFER_SIZE]; // 1 MiB buffer

    double end_time;
    double total_time;
    int i, x, y;
    double start_time = now();

#ifdef USE_FREAD
    FILE *fp;
    fp = fopen(file_name, "rb");
    for(i = 0; i < ITERATIONS; ++i)
    {
        fread(buffer, BUFFER_SIZE, 1, fp);
        for(x = 0; x < BUFFER_SIZE; x += 1024)
        {
            y += buffer[x];
        }
    }
    fclose(fp);

#elif USE_MMAP
    unsigned char *mmdata;
    int fd = open(file_name, O_RDONLY);
    for(i = 0; i < ITERATIONS; ++i)
    {
        mmdata = mmap(NULL, BUFFER_SIZE, PROT_READ, MAP_PRIVATE, fd, i * BUFFER_SIZE);
        // But if we don't touch it, it won't be read...
        // I happen to know I have 4 KiB pages, YMMV
        for(x = 0; x < BUFFER_SIZE; x += 1024)
        {
            y += mmdata[x];
        }
        munmap(mmdata, BUFFER_SIZE);
    }
    close(fd);

#else
    int fd;
    fd = open(file_name, O_RDONLY);
    for(i = 0; i < ITERATIONS; ++i)
    {
        read(fd, buffer, BUFFER_SIZE);
        for(x = 0; x < BUFFER_SIZE; x += 1024)
        {
            y += buffer[x];
        }
    }
    close(fd);

#endif

    end_time = now();
    total_time = end_time - start_time;

    printf("It took %f seconds to read 10 GiB. That's %f MiB/s.\n", total_time, ITERATIONS / total_time);

    return 0;
}
