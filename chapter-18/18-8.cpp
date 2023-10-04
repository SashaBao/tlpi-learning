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

int mynftw(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags) {
    DIR *dir = opendir(dirpath);
    struct dirent *p;
    while ((p = readdir(dir)) != nullptr) {
        if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0) {
            continue;
        }
        string buf(dirpath);
        buf += "/";
        buf += p->d_name;
        struct stat statbuf;
        if (stat(buf.c_str(), &statbuf) < 0) {
            perror("stat()");
        }
        if (S_ISDIR(statbuf.st_mode)) {
            mynftw(buf.c_str(), fn, nopenfd, flags);
        }
        fn(buf.c_str(), &statbuf, )
    }

}
