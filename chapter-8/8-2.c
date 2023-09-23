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
//version 1
struct passwd *mygetpwnam1(const char *name) {
    setpwent();
    struct passwd *pwd;
    while ((pwd = getpwent()) != NULL && strcmp(pwd->pw_name, name));
    endpwent();
    return pwd;
}

//version2
struct passwd *mygetpwnam2(const char *name) {
    setpwent();
    static struct passwd pwd;//分配的不是指针！
    struct passwd *tmp;
    while ((tmp = getpwent()) != NULL && strcmp(tmp->pw_name, name));//注意这里没有循环体 一旦其中一个条件破坏就停止
    if (tmp != NULL) {
        char *pw_name = (char *)malloc(strlen(tmp->pw_name) + 1); 
        strcpy(pw_name, tmp->pw_name);
        pwd.pw_name = pw_name;
        char *pw_passwd = (char *)malloc(strlen(tmp->pw_passwd) + 1); 
        strcpy(pw_passwd, tmp->pw_passwd);
        pwd.pw_passwd = pw_passwd;
        pwd.pw_uid = tmp->pw_uid;
        pwd.pw_gid = tmp->pw_gid;
        char *pw_gecos = (char *)malloc(strlen(tmp->pw_gecos) + 1); 
        strcpy(pw_gecos, tmp->pw_gecos);
        pwd.pw_gecos = pw_gecos;
        char *pw_dir = (char *)malloc(strlen(tmp->pw_dir) + 1); 
        strcpy(pw_dir, tmp->pw_dir);
        pwd.pw_dir = pw_dir;
        char *pw_shell = (char *)malloc(strlen(tmp->pw_shell) + 1); 
        strcpy(pw_shell, tmp->pw_shell);
        pwd.pw_shell = pw_shell;
    }
    endpwent();
    return tmp == NULL ? NULL : &pwd;
}
int main(int argc, char **argv) {
    struct passwd* pwd = mygetpwnam2("mayuri");
    if (pwd != NULL) {
        printf("%s;%s;%ld;%ld;%s;%s;%s\n", pwd->pw_name, pwd->pw_passwd, (long)pwd->pw_uid, (long)pwd->pw_gid, pwd->pw_gecos, pwd->pw_dir, pwd->pw_shell);
    }
}