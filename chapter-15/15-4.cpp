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
#include <sys/stat.h>
#include <algorithm>
#include <vector>
using namespace std;
//这里一开始愚蠢了
// int checkOtherMode(struct stat &statbuf, int mode) {
//     int flag = 1;
//     if (mode & R_OK) {
//         flag &= statbuf.st_mode & S_IROTH; 
//     }
//     if (mode & W_OK) {
//         flag &= statbuf.st_mode & S_IWOTH; 
//     }
//     if (mode & X_OK) {
//         flag &= statbuf.st_mode & S_IXOTH;
//     }
//     return flag;
// }
// 不应该用flag去& 因为右边的式子不一定在最低位为1！而flag由于是int 只有最低位为1 其他全为0
int checkOtherMode(struct stat &statbuf, int mode) {
    if (mode & R_OK && !(statbuf.st_mode & S_IROTH)) {
        return 0;
    }
    if (mode & W_OK && !(statbuf.st_mode & S_IWOTH)) {
        return 0;
    }
    if (mode & X_OK && !(statbuf.st_mode & S_IXOTH)) {
        return 0;
    }
    return 1;
}

int checkUserMode (struct stat &statbuf, int mode) {
    if (mode & R_OK && !(statbuf.st_mode & S_IRUSR)) {
        return 0;
    }
    if (mode & W_OK && !(statbuf.st_mode & S_IWUSR)) {
        return 0;
    }
    if (mode & X_OK && !(statbuf.st_mode & S_IXUSR)) {
        return 0;
    }
    return 1;
}

int checkGroupMode (struct stat &statbuf, int mode) {
    if (mode & R_OK && !(statbuf.st_mode & S_IRGRP)) {
        return 0;
    }
    if (mode & W_OK && !(statbuf.st_mode & S_IWGRP)) {
        return 0;
    }
    if (mode & X_OK && !(statbuf.st_mode & S_IXGRP)) {
        return 0;
    }
    return 1;
}
//第二种理解
int eaccess(const char *pathname, int mode) {
    uid_t euid = geteuid();
    if (euid == 0) {//特权级进程具有所有权限 直接返回1
        return 1;
    }
    gid_t egid = getegid();
    vector<gid_t> group(50);
    int numSupply = getgroups(100, group.data());//data可以使得vector退化为数组
    if (numSupply < 0) {
        perror("getgroups()");
    }
    group.resize(numSupply);//别忘了重新分配大小 之后的anyof需要end在正确的位置
    struct stat statbuf;//不用定义指针 传入stat取地址即可
    if (stat(pathname, &statbuf) < 0) {
        perror("stat()");
        if (mode & F_OK) {
            return 0;
        }
    }
    if (statbuf.st_uid == euid) {
        return checkUserMode(statbuf, mode);
    }
    if (statbuf.st_gid == egid || any_of(group.begin(), group.end(), [egid](gid_t gid){ //用到了stl 
        return gid == egid;
    }) ) {
        return checkGroupMode(statbuf, mode);
    }
    return checkOtherMode(statbuf, mode);
}

//第一种理解
int check(const char *pathname, int mode) {
    uid_t ruid = getuid();
    gid_t rgid = getgid();
    setuid(geteuid());
    setgid(getegid());
    int ret = access(pathname, mode);
    setuid(ruid);
    setgid(rgid);
    return ret;
}

int main(int argc, char *argv[]) {
    int fd = open("haha.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    char path[10] = "haha.txt";
    if (eaccess(path, R_OK)) {
        printf("you can read %s\n", path);
    }
    return 0;
}