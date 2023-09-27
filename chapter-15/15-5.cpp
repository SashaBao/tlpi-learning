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

bool isAnyExe(mode_t &mode) {
    if (mode & S_IXOTH || mode & S_IXUSR || mode & S_IXGRP) {
        return true;
    }
    return false;
}
int mychmod(const char *pathname) {
    struct stat statbuf;
    mode_t mode;
     if (stat(pathname, &statbuf) < 0) {
        perror("stat()");
        return -1;
    }
    //statbuf.st_mode |= S_IROTH | S_IWOTH | S_IXOTH;
    mode = statbuf.st_mode;
    mode |= S_IROTH | S_IRUSR | S_IRGRP;
    if (S_ISDIR(statbuf.st_mode) || isAnyExe(statbuf.st_mode)) {
        //statbuf.st_mode |= S_IXOTH | S_IXUSR | S_IXGRP;
        // chmod(pathname, statbuf.st_mode | S_IXOTH | S_IXUSR | S_IXGRP);
        mode |= S_IXOTH | S_IXUSR | S_IXGRP;
    }
    return chmod(pathname, mode);
}

int main(int argc, char *argv[]) {
    mychmod(argv[1]);
    mychmod(argv[2]);
    mychmod(argv[3]);
}