#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int mydup(int fd) {
    return fcntl(fd, F_DUPFD, 0);
}

int mydup2(int oldfd, int newfd) {
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) == -1) { //����ж���ʵӦ�õ����ó�������ǰ�棬��Ϊoldfd������Ч�����Ӧ���������жϵ�
            return -1;
        }
        return oldfd;
    }

    if (fcntl(newfd, F_GETFL) != -1) {
        close(newfd);
    }
    return fcntl(oldfd, F_DUPFD, newfd);

}
int main(int argc, char *argv[]) {
    int oldfd, newfd;
	newfd = mydup(0);
	printf("oldfd : 0, newfd : %d\n", newfd);
	newfd = mydup2(0, 4);
	printf("oldfd : 0, newfd : %d\n", newfd);
	return 0;
}