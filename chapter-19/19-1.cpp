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
#include <sys/inotify.h>
#include <ftw.h>
#include <unordered_map>
using namespace std;
int inotifyFd = inotify_init(); //fn无法加入其他参数
unordered_map<int, string> map; //line 55 需要绝对路径 存储wd->path的映射
int fn (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    int wd;
    if (typeflag == FTW_D) {
        wd = inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF);
        if (wd < 0) {
            perror("inotify_add_watch");
            return -1;
        }
        cout<<fpath<<" has been added to inotifyFd"<<endl;
    }
    return 0;
}

void process(struct inotify_event *p) {
    if (p->len == 0 && p->mask & IN_DELETE_SELF) {
        inotify_rm_watch(inotifyFd, p->wd);
        cout<<"a wd has been deleted from inotifyFD"<<endl;
        return;
    }
    
    if (!(p->len != 0 && p->mask & IN_CREATE)) {//never nester
        return;
    }
    struct stat statbuf;
    string path(map[p->wd]);
    path += "/";
    path += p->name;
    if (stat(path.c_str(), &statbuf) < 0) {
        perror("stat()");
    }
    if (S_ISDIR(statbuf.st_mode)) {
        int wd = inotify_add_watch(inotifyFd, path.c_str(), IN_CREATE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF);
        if (wd < 0) {
            perror("inotify_add_watch");
        }
    }
    cout<<"a new wd has been added to inotifyFD"<<endl;
    
}
int main(int argc, char **argv) {
    int wd, numRead;
    char buf[4096];
    struct inotify_event *event;
    if (inotifyFd < 0) {
        perror("inotify_init()");
    }
    for (int i = 1; i < argc; ++i) { // &是不算在argc当中的
        int wd = inotify_add_watch(inotifyFd, argv[i], IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MOVE_SELF);
        if (wd < 0) {
            perror("inotify_add_watch");
        }
        map[wd] = argv[i];
        nftw(argv[i], fn, 10, 0);
    }

    while (1) {
        cout<<"reading..."<<endl;
        numRead = read(inotifyFd, buf, sizeof(buf));
        if (numRead < 0) {
            perror("read()");
        }
        for (char *p = buf; p < buf + numRead; ) {
            auto event = reinterpret_cast<struct inotify_event *> (p); //区分C++ 四种动态类型转化
            process(event);
            p += sizeof (struct inotify_event) + event->len; //移动到下一个事件
        }
    }
}