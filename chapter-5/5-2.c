#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd;
    if ((fd = open("1.txt", O_WRONLY | O_APPEND)) == -1) {
        perror("open");
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
    };
    if (write(fd, "text", 4) != 4) {
        perror("write");
    }
    if (close(fd) == -1) {
        perror("close");
    }
    return 0;
}