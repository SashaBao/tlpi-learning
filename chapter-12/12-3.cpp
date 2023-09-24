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

//������д�����ͬ�ļ����Ǿͻ��pid��Ӧ���̵���Ϣ
string getProcInfo(string &pid) {
    string path("/proc/" + pid + "/status");
    string line, key, value;
    ifstream file(path); // ʹ�� std::ifstream ���ļ�
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
    file.close(); // ���˳�������ʱ��std::ifstream ���Զ��ر��ļ�
    //����Ҫreturn �϶�����Name
}

//����ĳ��pid��fdĿ¼��������û�д���ͬ���ļ�
string parsefd(string &path, string comparedStr, string pid) {
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir(path.c_str()), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }
    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {
        if (isInteger(p->d_name) == 0) { //fdĿ¼��ֻ���ļ������� ��Ȼ������ still needs
            continue;
        }
        string tmpPath(path);//������һ��ʼ������ȡ����path�����뺯������һ����
        //��ʵpath��Ӧ�ö����ں����ڲ� ��Ϊÿ�ּӵĶ�������һ��
        tmpPath += "/";
        tmpPath += p->d_name;
        string buf(1024, '\0');
        if (readlink(tmpPath.c_str(), &buf[0], buf.size()) == -1) {
            perror("readlink()");
        }
        // cout<<buf.size()<<'\t'<<comparedStr.size()<<endl;
        if (buf.substr(0, comparedStr.size()) == comparedStr) {//string�Ƚ� ��ʹ����һ�� ��С��һ��Ҳ�ᱨ��
            // cout<<buf<<endl;
            return getProcInfo(pid);
        }
    }
    return "";

}

int main(int argc, char **argv) {
    // int fd = open("/home/mayuri/�ĵ�/12/1.txt", O_RDONLY);
    // if (fd == -1) {
    //     perror("open()");
    // }
    // cout<<fd<<endl;
    unique_ptr<DIR, decltype(DirCloser)> dir(opendir("/proc"), DirCloser);
    if (dir == nullptr) {
        perror("opendir()");
    }
    cout<<"����"<<argv[1]<<"�Ľ����У�"<<endl;
    struct dirent *p;
    while ((p = readdir(dir.get())) != NULL) {//get() returns primitive pointer
        if (isInteger(p->d_name) == 0) {
            continue;
        }
        string path("/proc/");
        path += p->d_name;
        path += "/fd";
        string tmp = parsefd(path, argv[1], p->d_name);//����дdebug��ֱ�ӱ��� ��ʹ�������parsefd�ڲ�
        //��Ϊͬʱ��������+��ֵ ��ʹsҲ������뺯���ڲ�
        //�ֿ�д������debug
        if (tmp != "") {
            cout<<tmp<<' '<<"PID:"<<p->d_name<<endl;
        }
    }
    // close(fd);
}

