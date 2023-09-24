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

#define MAXLINE 4396

int isInteger(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); //尝试将字符串转换为整数（十进制）
    
    // 如果 endptr 指向字符串的结束字符 '\0'，表示转换成功
    // 并且如果 str 不为空字符串（*str != '\0'），表示字符串不仅仅是空格
    return *endptr == '\0' && *str != '\0';
}

uid_t userIdFromName(const char *name) {
    struct passwd *pwd;
    uid_t u;
    char *endptr;
    if (name == NULL || *name == '\0') {
        return -1;
    }
    u = strtol(name, &endptr, 10);
    if (*endptr == '\0') {
        return u;
    }
    pwd = getpwnam(name);
    if (pwd == NULL) {
        return -1;
    }
    return pwd->pw_uid;
}

char *trim(char *str) {
    int start = 0, end = strlen(str) - 1;

    //去除开头的空白字符
    while (isspace((unsigned char)str[start])) {
        start++;
    }

    //去除结尾的空白字符
    while (end > start && isspace((unsigned char)str[end])) {
        end--;
    }

    //将结尾的空白字符替换为 null 终止符
    str[end + 1] = '\0';

    // 移动修剪后的字符串到字符串开头
    if (start > 0) {
        int i = 0;
        while (str[start + i] != '\0') {
            str[i] = str[start + i];
            i++;
        }
        str[i] = '\0';
    }
    return str;
}

void parseProcessStatus(char *path, char *name) {
    FILE *fp;
    char *key, *value;
    char userIdStr[MAXLINE];
    char linebuf[MAXLINE];

    // printf("%s", path);
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen()");
    }
    sprintf(userIdStr, "%d", userIdFromName(name));
    while ((fgets(linebuf, MAXLINE, fp)) != NULL) { //fgets遇到换行符也会返回
        key = strtok(linebuf, ":");//strtok的man好好读
        value = strtok(NULL, ":");
        key = trim(key);//去除掉空白符
        value = trim(value);
        if (strcmp(key, "Uid") == 0 && strstr(value, userIdStr) != NULL) {//只要uid出现在Uid中 就认为是该用户运行的
            printf("Pid:%s, Name:%s\n", pid, cmdLine); //因为题目要求输出pid 和 Name 而这两项都出现在Uid之前
        }
        if (strcmp(key, "Uid") == 0 && strstr(value, userIdStr) == NULL) { //不在直接退出循环
            break;
        }
        if (strcmp(key, "Pid") == 0) {
            char *pid = (char *)malloc(strlen(value) + 1);//动态申请 减少内存开销
            strcpy(pid, value);
        }
        if (strcmp(key, "Name") == 0) {
            char *cmdLine = (char *)malloc(strlen(value) + 1);//动态申请 减少内存开销
            strcpy(cmdLine, value);
        }
    }
    free(cmdLine);//因为Uid在Pid与Name之后 所以无论成功失败 一定动态分配了
    free(pid);
}

int main(int argc, char **argv) {
    int fd;
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir()");
    }

    struct dirent *p = readdir(dir);
    while ((p = readdir(dir)) != NULL) {
        if (isInteger(p->d_name) == 0) { //排除掉每个目录都有的.和.. 而且proc目录下还存在一些与进程无关的系统信息 文件名不像进程一样都是数字
            continue;
        }
        char *path = (char *)malloc(6 + strlen(p->d_name + 7 + 1));// /proc 6 /status 7 /0 1
        strcpy(path, "/proc/");
        strcat(path, p->d_name);
        strcat(path, "/status");
        parseProcessStatus(path, argv[1]);
        free(path);//动态申请的要释放
    }

}