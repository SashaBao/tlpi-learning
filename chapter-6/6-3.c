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
// setenv()函数为形如 name=value 的字符串分配一块内存缓冲区，并将 name 和 value 所指
// 向的字符串复制到此缓冲区，以此来创建一个新的环境变量。注意，不需要（实际上，是绝
// 对不要）在 name 的结尾处或者 value 的开始处提供一个等号字符，因为 setenv()函数会在向环
// 境添加新变量时添加等号字符。
int mysetenv(const char *name, const char *value, int overwrite) {
    char *tmp = getenv(name);
    if (tmp != NULL && overwrite == 0) {
        return 0;
    }
    
    if (tmp != NULL && overwrite) {
        memcpy(tmp, value, strlen(value) + 1);//拷贝/0
        return 0;
    }
    int nameSize = strlen(name), valueSize = strlen(value);
    char *p = (char *)malloc(nameSize + valueSize + 2); //不仅要为=留位置 还要给结束符留位置
    //我之前只加了1 侥幸对了 是因为默认初始化就是\0 而且分配的内存区域后面也是空闲的 
    if (p == NULL) {
        return -1;
    }
    memcpy(p, name, nameSize);
    memcpy(p + nameSize, "=", 1);//cant be '='
    memcpy(p + nameSize + 1, value, valueSize + 1);//拷贝/0
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
                *tmp = *(tmp + 1); //*tmp = *(tmp++ + 1);是错的 更新的将会是++后的tmp
                ++tmp;//while一定要注意更新循环变量
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