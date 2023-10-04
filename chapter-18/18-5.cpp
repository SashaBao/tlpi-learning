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
#include <string>
#include <filesystem>
#include <memory>
#include <libgen.h>
#include <iostream>
using namespace std;
bool isDevAndInoEqual(struct stat &a, struct stat &b) {
    return a.st_dev == b.st_dev && a.st_ino == b.st_ino;
}

string search(struct stat &searched) {
    // long searchedDev = dynamic_cast<long>searched.st_dev;
    // long searchedIno = dunamic_cast<long>searched.st_ino;
    DIR *dir = opendir("..");
    struct dirent *p;
    while ((p = readdir(dir)) != nullptr) {
        if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0) {
            continue;
        }
        string buf("../");//循环内定义 不然路径不对
        buf += p->d_name;
        struct stat statbuf;
        if (stat(buf.c_str(), &statbuf) < 0) {
            perror("stat()");
        }
        if (isDevAndInoEqual(searched, statbuf)) {
            return buf.substr(3);
        }
    }

}

char *mygetcwd(char *buf, size_t size) {
    int originalWorkingDir = open(".", O_RDONLY);
    vector<string> result;
    while (1) {
        //检查父目录是否与当前工作目录相同
        struct stat currentDir, parentDir;
        if (stat(".", &currentDir) < 0) {
            perror("stat()");
        }
        if (stat("..", &parentDir) < 0) {
            perror("stat()");
        }
        if (isDevAndInoEqual(currentDir, parentDir)) {
            break;
        }
        result.emplace_back(search(currentDir));
        chdir("..");
    }
    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        strncat(buf, it->c_str(), it->size());
        if (it == result.rend() - 1) {//可以看循环变量是++ 那么肯定是- 这与直觉不太一样 
            continue;
        }
        strncat(buf, "/", 1);
    } 
    fchdir(originalWorkingDir);
    return buf;
    
}

int main() {
    char buf1[PATH_MAX], buf2[PATH_MAX];
    buf1[0] = '\0';
    buf2[0] = '\0';//strncat根据\0增加 默认初始化会出现怪东西
    cout<<"getcwd:"<<getcwd(buf1, PATH_MAX)<<endl;
    cout<<"mygetcwd:"<<mygetcwd(buf2, PATH_MAX)<<endl;
}