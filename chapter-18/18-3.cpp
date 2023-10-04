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

char *myrealpath(const char *pathname, char *resolved_path) {
    if (resolved_path == nullptr) {
        unique_ptr<char[]> resolved_path(new char[PATH_MAX]);
    }
    unique_ptr<char> pathCopy1 {strdup(pathname)};//strdup�ķ���ֵ�ڶ��� 
    unique_ptr<char> pathCopy2 {strdup(pathname)};//dirname��basename���п��ܸı�ԭ���ַ��� ���뿽������

    char *dir = dirname(pathCopy1.get());
    char *base = basename(pathCopy2.get());
    // cout<<dir<<" "<<base<<endl;
    struct stat statbuf;
    char buf[PATH_MAX];
    if (lstat(pathname, &statbuf) < 0) { //Ҫ��chdir֮ǰlstat ��Ȼ���Ҳ���pathname��
        perror("lstat()");
    }
    int cur_fd = open(".", O_RDWR); //һ������˫����
    chdir(dir);
    //cout<<pathCopy.get()<<endl;
    

    if (S_ISLNK(statbuf.st_mode)) {
        int numRead = readlink(pathname, buf, PATH_MAX);
        if (numRead == -1) {
            perror("readlink()");
        }
        buf[numRead] = '\0'; //readlink�������Ľ�����
        if (buf[0] == '/') {//��Ŀ¼ ������·��
            // strncat(resolved_path, buf, numRead);
            sprintf(resolved_path, "%s", buf);
        } else { //�Ǹ�Ŀ¼ �����·�� ��ʱ����ڸ�Ŀ¼
            // strncat(resolved_path, dir, sizeof(dir));
            // strncat(resolved_path, buf, numRead);
            sprintf(resolved_path, "%s/%s", dir, buf);//ע���б��
        }
        return myrealpath(resolved_path, resolved_path);//��������ָ�����Ȼ�����Ƿ�������
        //sprintf���resolved_path���и��� 
    } else { //Ŀ¼or�����ļ�
        sprintf(resolved_path, "%s/%s", getcwd(buf, PATH_MAX), base);//ע���б��
    }
    fchdir(cur_fd);
    close(cur_fd);
    return resolved_path;
    // if (path[0] == '/') {
    //     auto end = path.find_first_of('/', 1)
    // } else {
    //     auto end = path.find_first_of('/');
    // }
    // //auto start = path.find_first_not_of('\\');
    // while (start != string::npos || end != string::npos) {
    //     string token{string.substr(0, end)};
    //     if (token == ".") {
    //         result += getcwd()
    //     }
    //     start = path.find_first_not_of('\\', end);
    //     end = path.find_first_of('\\', start);
    // }
}

int main(int argc, char **argv) {
    char resolved_path[PATH_MAX];
    cout<<myrealpath(argv[1], resolved_path)<<endl;
}