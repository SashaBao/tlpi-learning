#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd, numBytes;
    if (argc == 3) {
        fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
    } else {
        fd = open(argv[1], O_CREAT | O_WRONLY, 0666);
    }
    if (fd == -1) {
        perror("open");
    }
    numBytes = atoi(argv[2]);
    while (numBytes--) {
        if (argc == 4) {
            lseek(fd, 0, SEEK_END);
            
        } 
        write(fd, "m", 1);
    }
}