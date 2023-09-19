#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
extern char **environ;
// setenv()����Ϊ���� name=value ���ַ�������һ���ڴ滺���������� name �� value ��ָ
// ����ַ������Ƶ��˻��������Դ�������һ���µĻ���������ע�⣬����Ҫ��ʵ���ϣ��Ǿ�
// �Բ�Ҫ���� name �Ľ�β������ value �Ŀ�ʼ���ṩһ���Ⱥ��ַ�����Ϊ setenv()����������
// ������±���ʱ��ӵȺ��ַ���
int mysetenv(const char *name, const char *value, int overwrite) {
    char *tmp = getenv(name);
    if (tmp != NULL && overwrite == 0) {
        return 0;
    }
    
    if (tmp != NULL && overwrite) {
        memcpy(tmp, value, strlen(value) + 1);//����/0
        return 0;
    }
    int nameSize = strlen(name), valueSize = strlen(value);
    char *p = (char *)malloc(nameSize + valueSize + 2); //����ҪΪ=��λ�� ��Ҫ����������λ��
    //��֮ǰֻ����1 ���Ҷ��� ����ΪĬ�ϳ�ʼ������\0 ���ҷ�����ڴ��������Ҳ�ǿ��е� 
    if (p == NULL) {
        return -1;
    }
    memcpy(p, name, nameSize);
    memcpy(p + nameSize, "=", 1);//cant be '='
    memcpy(p + nameSize + 1, value, valueSize + 1);//����/0
    if (putenv(p)) {
        return -1;
    }
    return 0;
}

//wrong version
// int myunsetenv(const char *name) {
//     char *tmp;
//     while ((tmp = getenv(name)) != NULL) {
//         free(tmp - 1 - strlen(name));
//     }
//     return 0;
// }

int myunsetenv(const char *name) {
    char **ep;
    for (ep = environ; *ep != NULL; ep++) {
        if (strncmp(*ep, name, strlen(name)) == 0) {
            char **tmp = ep;
            while (*tmp != NULL) {
                *tmp = *(tmp + 1); //*tmp = *(tmp++ + 1);�Ǵ�� ���µĽ�����++���tmp
                ++tmp;//whileһ��Ҫע�����ѭ������
            }
        }
    }
}

int main(int argc, char **argv) {
    if (mysetenv("eren", "yeager", 0) == -1) {
        fprintf(stderr, "putenv error");
    }
    if (mysetenv("mikasa", "ackerman", 0) == -1) {
        fprintf(stderr, "putenv error");
    }
    if (mysetenv("eren", "ackerman", 1) == -1) {
        fprintf(stderr, "putenv error");
    }
    printf("%s\n", getenv("eren"));
    printf("%s\n", getenv("mikasa"));
    myunsetenv("eren");
    char *tmp = getenv("eren");
    if (tmp == NULL) {
        printf("delete successfully!\n");
    } else {
        printf("fail to delete!\n");
    }

}