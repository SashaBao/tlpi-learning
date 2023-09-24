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
    strtol(str, &endptr, 10); //���Խ��ַ���ת��Ϊ������ʮ���ƣ�
    
    // ��� endptr ָ���ַ����Ľ����ַ� '\0'����ʾת���ɹ�
    // ������� str ��Ϊ���ַ�����*str != '\0'������ʾ�ַ����������ǿո�
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

    //ȥ����ͷ�Ŀհ��ַ�
    while (isspace((unsigned char)str[start])) {
        start++;
    }

    //ȥ����β�Ŀհ��ַ�
    while (end > start && isspace((unsigned char)str[end])) {
        end--;
    }

    //����β�Ŀհ��ַ��滻Ϊ null ��ֹ��
    str[end + 1] = '\0';

    // �ƶ��޼�����ַ������ַ�����ͷ
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
    while ((fgets(linebuf, MAXLINE, fp)) != NULL) { //fgets�������з�Ҳ�᷵��
        key = strtok(linebuf, ":");//strtok��man�úö�
        value = strtok(NULL, ":");
        key = trim(key);//ȥ�����հ׷�
        value = trim(value);
        if (strcmp(key, "Uid") == 0 && strstr(value, userIdStr) != NULL) {//ֻҪuid������Uid�� ����Ϊ�Ǹ��û����е�
            printf("Pid:%s, Name:%s\n", pid, cmdLine); //��Ϊ��ĿҪ�����pid �� Name �������������Uid֮ǰ
        }
        if (strcmp(key, "Uid") == 0 && strstr(value, userIdStr) == NULL) { //����ֱ���˳�ѭ��
            break;
        }
        if (strcmp(key, "Pid") == 0) {
            char *pid = (char *)malloc(strlen(value) + 1);//��̬���� �����ڴ濪��
            strcpy(pid, value);
        }
        if (strcmp(key, "Name") == 0) {
            char *cmdLine = (char *)malloc(strlen(value) + 1);//��̬���� �����ڴ濪��
            strcpy(cmdLine, value);
        }
    }
    free(cmdLine);//��ΪUid��Pid��Name֮�� �������۳ɹ�ʧ�� һ����̬������
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
        if (isInteger(p->d_name) == 0) { //�ų���ÿ��Ŀ¼���е�.��.. ����procĿ¼�»�����һЩ������޹ص�ϵͳ��Ϣ �ļ����������һ����������
            continue;
        }
        char *path = (char *)malloc(6 + strlen(p->d_name + 7 + 1));// /proc 6 /status 7 /0 1
        strcpy(path, "/proc/");
        strcat(path, p->d_name);
        strcat(path, "/status");
        parseProcessStatus(path, argv[1]);
        free(path);//��̬�����Ҫ�ͷ�
    }

}