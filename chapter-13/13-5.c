#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <ctype.h>

#define BUFFERSIZE (1 << 12)
int main(int argc, char **argv) {
    int num = argc > 2 ? atoi(argv[2]) : 10;
    char buf[BUFFERSIZE];
    int fd = open(argv[3], O_RDONLY);
    int countOfNewline = 0, numRead;
    off_t offsetOfNewline[BUFFERSIZE];//记录每个\n对应的偏移量
    off_t base = 0;//记录偏移量光循环变量还不够 
    while ((numRead = read(fd, buf, BUFFERSIZE)) > 0) {
        for (int i = 0; i < numRead; ++i) {
            if (buf[i] == '\n') {
                offsetOfNewline[countOfNewline++] = i + base;
            }
        }
        base += numRead;
    }
    lseek(fd, -1, SEEK_END);//考虑文件末尾是否有换行
    read(fd, buf, 1);
    if (buf[0] != '\n') {
        ++countOfNewline;
    }
    if (countOfNewline <= num) { 
        lseek(fd, 0, SEEK_SET);
    } else {
        int finalOff = countOfNewline - num - 1;//可以自己举例子
        lseek(fd, offsetOfNewline[finalOff] + 1, SEEK_SET);
    }
    while ((numRead = read(fd, buf, BUFFERSIZE)) > 0) {
        write(STDOUT_FILENO, buf, numRead);
    }
}