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
#include <iomanip> // 添加此头文件以进行缩进

using namespace std;

using string_cmd = string;//因为pair的两个都是string 容易让人分不清楚
using string_pid = string;//一开始string_pid我用的是pid_t的类型 但是我发现没用 用的时候还是要转化成pid_t 干脆直接string

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

//检查某个进程的父进程是不是pidStr
pair<string_cmd, string_pid> parseProcessStatus(string path, string pidStr) {
    string key, value;
    string line, cmdline, pid;
    ifstream file(path); // 使用 std::ifstream 打开文件
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            key = line.substr(0, pos);
            value = line.substr(pos + 1);
        }
        key = trim(key);
        value = trim(value);
        if (key == "PPid" && value.find(pidStr) != string::npos) {
            return {cmdline, pid};
        }
        if (key == "Name") {
            cmdline = value;
        }

        if (key == "Pid") {
            pid = value;
        }
    }
    file.close(); // 在退出作用域时，std::ifstream 会自动关闭文件
    return {"", ""};
}

//找到pidStr对应进程的所有子进程 并存入vector中
void findChildren(const string &pidStr, vector<pair<string_cmd, string_pid>> &childs) {
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir("/proc"), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }

    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {//.get()可以返回原始指针
        if (isInteger(p->d_name) == 0) {
            continue;
        }
        string path("/proc/");
        path += p->d_name;
        path += "/status";
        pair<string_cmd, string_pid> tmp = parseProcessStatus(path, pidStr);
        // tmp != {"",""}
        // tmp != make_pair("","")
        //上述两种写法都是错误的 
        if (tmp.first != "" && tmp.second != "") {
            childs.emplace_back(tmp);
        }
    }
}

// 最初版本
// void dfs(vector<pair<string_cmd, string_pid>> &childs) {
//     if (childs.empty()) {
//         cout<<endl;
//         return;
//     }
//     size_t numChild = childs.size();
//     for (const auto &child : childs) {
//         cout<<"|->";
//         cout<<child.first;
//         cout<<"\t\t";
//         vector<pair<string_cmd, string_pid>> childs;
//         findChildren(child.second, childs);
//         dfs(childs);
//         cout<<"\t\t";
//     }

// }

//这里的递归自己好好品味下吧
void dfs(vector<pair<string_cmd, string_pid>> &childs, int depth) {
    if (childs.empty()) {
        return;
    }

    size_t numChild = childs.size();
    for (size_t i = 0; i < numChild; ++i) {
        const auto &child = childs[i];
        cout << setw(4 * depth) << ""; // 缩进输出，每级缩进4个空格
        cout << "|-> " << child.first << " (PID: " << child.second << ")" << endl;
        vector<pair<string_cmd, string_pid>> grandchildren;
        findChildren(child.second, grandchildren);
        dfs(grandchildren, depth + 1);
    }
}

int main(int argc, char **argv) {
    vector<pair<string_cmd, string_pid>> childs;
    childs.emplace_back("systemd", "1");
    dfs(childs, 0); // 从根节点开始，初始深度为0
}
