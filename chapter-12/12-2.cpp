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
#include <iomanip> // ��Ӵ�ͷ�ļ��Խ�������

using namespace std;

using string_cmd = string;//��Ϊpair����������string �������˷ֲ����
using string_pid = string;//һ��ʼstring_pid���õ���pid_t������ �����ҷ���û�� �õ�ʱ����Ҫת����pid_t �ɴ�ֱ��string

auto DirCloser = [](DIR *dir) {
    if (closedir(dir) != 0) {
        perror("closedir()");
    }
};

int isInteger(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10); // ���Խ��ַ���ת��Ϊ������ʮ���ƣ�
    
    // ��� endptr ָ���ַ����Ľ����ַ� '\0'����ʾת���ɹ�
    // ������� str ��Ϊ���ַ�����*str != '\0'������ʾ�ַ����������ǿո�
    return *endptr == '\0' && *str != '\0';
}

string trim(const string& str) {
    size_t start = 0;
    size_t end = str.length();

    // ���ҵ�һ���ǿո��ַ���λ��
    while (start < end && isspace(str[start])) {
        start++;
    }

    // �������һ���ǿո��ַ���λ��
    while (end > start && isspace(str[end - 1])) {
        end--;
    }

    // �����޼�������ַ���
    return str.substr(start, end - start);
}

//���ĳ�����̵ĸ������ǲ���pidStr
pair<string_cmd, string_pid> parseProcessStatus(string path, string pidStr) {
    string key, value;
    string line, cmdline, pid;
    ifstream file(path); // ʹ�� std::ifstream ���ļ�
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
    file.close(); // ���˳�������ʱ��std::ifstream ���Զ��ر��ļ�
    return {"", ""};
}

//�ҵ�pidStr��Ӧ���̵������ӽ��� ������vector��
void findChildren(const string &pidStr, vector<pair<string_cmd, string_pid>> &childs) {
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir("/proc"), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }

    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {//.get()���Է���ԭʼָ��
        if (isInteger(p->d_name) == 0) {
            continue;
        }
        string path("/proc/");
        path += p->d_name;
        path += "/status";
        pair<string_cmd, string_pid> tmp = parseProcessStatus(path, pidStr);
        // tmp != {"",""}
        // tmp != make_pair("","")
        //��������д�����Ǵ���� 
        if (tmp.first != "" && tmp.second != "") {
            childs.emplace_back(tmp);
        }
    }
}

// ����汾
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

//����ĵݹ��Լ��ú�Ʒζ�°�
void dfs(vector<pair<string_cmd, string_pid>> &childs, int depth) {
    if (childs.empty()) {
        return;
    }

    size_t numChild = childs.size();
    for (size_t i = 0; i < numChild; ++i) {
        const auto &child = childs[i];
        cout << setw(4 * depth) << ""; // ���������ÿ������4���ո�
        cout << "|-> " << child.first << " (PID: " << child.second << ")" << endl;
        vector<pair<string_cmd, string_pid>> grandchildren;
        findChildren(child.second, grandchildren);
        dfs(grandchildren, depth + 1);
    }
}

int main(int argc, char **argv) {
    vector<pair<string_cmd, string_pid>> childs;
    childs.emplace_back("systemd", "1");
    dfs(childs, 0); // �Ӹ��ڵ㿪ʼ����ʼ���Ϊ0
}
