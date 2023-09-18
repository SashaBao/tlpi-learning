#define _FILE_OFFSET_BITS 64
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd;
    off_t off;
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    off = atoll(argv[2]);
    lseek(fd, off, SEEK_SET);
    write(fd, "test", 4);
    exit(0);
}