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
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;

auto DirCloser = [](DIR *dir) {
    if (closedir(dir) != 0) {
        perror("closedir()");
    }
};

int isInteger(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); // 尝试将字符串转换为整数（十进制）
    
    // 如果 endptr 指向字符串的结束字符 '\0'，表示转换成功
    // 并且如果 str 不为空字符串（*str != '\0'），表示字符串不仅仅是空格
    return *endptr == '\0' && *str != '\0';
}

string trim(const string& str) {
    size_t start = 0;
    size_t end = str.length();

    // 查找第一个非空格字符的位置
    while (start < end && isspace(str[start])) {
        start++;
    }

    // 查找最后一个非空格字符的位置
    while (end > start && isspace(str[end - 1])) {
        end--;
    }

    // 返回修剪后的子字符串
    return str.substr(start, end - start);
}

//如果具有打开了相同文件，那就获得pid对应进程的信息
string getProcInfo(string &pid) {
    string path("/proc/" + pid + "/status");
    string line, key, value;
    ifstream file(path); // 使用 std::ifstream 打开文件
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            key = line.substr(0, pos);
            value = line.substr(pos + 1);
        }
        key = trim(key);
        value = trim(value);
        if (key == "Name") {
            return value;
        }
    }
    file.close(); // 在退出作用域时，std::ifstream 会自动关闭文件
    //不需要return 肯定包含Name
}

//遍历某个pid的fd目录，看看有没有打开相同的文件
string parsefd(string &path, string comparedStr, string pid) {
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir(path.c_str()), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }
    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {
        if (isInteger(p->d_name) == 0) { //fd目录中只有文件描述符 必然是数字 still needs
            continue;
        }
        string tmpPath(path);//这里我一开始变量名取成了path。。与函数参数一样了
        //其实path就应该定义在函数内部 因为每轮加的东西都不一样
        tmpPath += "/";
        tmpPath += p->d_name;
        string buf(1024, '\0');
        if (readlink(tmpPath.c_str(), &buf[0], buf.size()) == -1) {
            perror("readlink()");
        }
        // cout<<buf.size()<<'\t'<<comparedStr.size()<<endl;
        if (buf.substr(0, comparedStr.size()) == comparedStr) {//string比较 即使内容一样 大小不一样也会报错
            // cout<<buf<<endl;
            return getProcInfo(pid);
        }
    }
    return "";

}

int main(int argc, char **argv) {
    // int fd = open("/home/mayuri/文档/12/1.txt", O_RDONLY);
    // if (fd == -1) {
    //     perror("open()");
    // }
    // cout<<fd<<endl;
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir("/proc"), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }
    cout<<"运行"<<argv[1]<<"的进程有："<<endl;
    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {//get() returns primitive pointer
        if (isInteger(p->d_name) == 0) {
            continue;
        }
        string path("/proc/");
        path += p->d_name;
        path += "/fd";
        string tmp = parsefd(path, argv[1], p->d_name);//这样写debug会直接报错 即使错误出在parsefd内部
        //因为同时函数调用+赋值 即使s也不会进入函数内部
        //分开写有助于debug
        if (tmp != "") {
            cout<<tmp<<' '<<"PID:"<<p->d_name<<endl;
        }
    }
    // close(fd);
}

