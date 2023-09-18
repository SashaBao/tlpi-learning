#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int oldfd, newfd, flags, accessMode, newseek;
    oldfd = open("1.txt", O_CREAT | O_APPEND | O_WRONLY, 0666);
    //这里之前没加0666的时候出现过open error和dup error 但是加了之后我写完了代码 之后又删掉 测试了好几次也没出现open dup error 不知道为什么
    lseek(oldfd, 2, SEEK_SET);
    if (oldfd == -1) {
        perror("open");
    }
    newfd = dup(oldfd);
    if (newfd == -1) {
        perror("dup");
    }

    flags = fcntl(newfd, F_GETFL);
    accessMode = flags & O_ACCMODE;
    if ((accessMode == O_WRONLY) && (flags & O_APPEND)) {
        printf("status flags are the same!\n");
    } else {
        printf("status flags are not the same!\n");
    }

    newseek = lseek(newfd, 0, SEEK_CUR);
    if (newseek == lseek(oldfd, 0, SEEK_CUR)) {
        printf("offset same!\n");
    } else {
        printf("offset not the same!\n");
    }


}