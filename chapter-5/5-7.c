#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <string.h>
#define STRSIZE 100
ssize_t myreadv(int fd, const struct iovec *iov, int iovcnt) {
    int numRead, numWritten, totalRead = 0;
    for (int i = 0; i < iovcnt; ++i) {
        int bufsize = iov[i].iov_len;
        numRead = read(fd, iov[i].iov_base, bufsize);
        if (numRead == -1) {
            return -1;
        }
        if (numRead == 0) { //reach EOF
            break;
        }
        totalRead += numRead;
    }
    return totalRead;
}

//mallocһ���󻺳�����iov����ȫ��д��ô󻺳������ٽ��󻺳���д���ļ�
ssize_t mywritev(int fd, const struct iovec *iov, int iovcnt) {
    
    int numWritten, totalWrite = 0;
    for (int i = 0; i < iovcnt; i++) {
        totalWrite += iov[i].iov_len;
    }
    char *p = (char *)malloc(totalWrite);
    char *tmp = p;//p���ƶ� ȷ��ȫ��д������ݲ��ᱻ���渲�� ��Ҫfree�Ļ���Ҫ��ס�����ָ��
    for (int i = 0; i < iovcnt; i++) {
        memcpy(p, iov[i].iov_base, iov[i].iov_len);
        p += iov[i].iov_len;
    }
    
    if ((numWritten = write(fd, tmp, totalWrite)) == -1) {
        return -1;       
    }
    free(tmp);

    return numWritten;
}
int main(int argc, char *argv[]) {
    int infd, outfd, numRead, numWritten;
    
    infd = open("content.txt", O_RDONLY, 0666);//content.txt exists in advance
    if (infd == -1) {
        perror("open");
    }
    //according to the book
    struct iovec iov[3];
    struct stat myStruct;
    int x;
    char str[STRSIZE];
    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    iov[2].iov_base = str;
    iov[2].iov_len = STRSIZE;

    numRead = readv(infd, iov, 3);
    if (numRead == -1) {
        perror("read");//mywritev��myreadv���� ��ʵ�������з�װ��write��read������ ����������errno
    }
    outfd = open("output.txt", O_WRONLY | O_CREAT, 0666);
    if (outfd == -1) {
        perror("open");
    }
    numWritten = writev(outfd, iov, 3);
    if (numWritten == -1) {
        perror("write");
    }
    return 0;
}