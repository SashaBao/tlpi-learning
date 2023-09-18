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
    //����֮ǰû��0666��ʱ����ֹ�open error��dup error ���Ǽ���֮����д���˴��� ֮����ɾ�� �����˺ü���Ҳû����open dup error ��֪��Ϊʲô
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