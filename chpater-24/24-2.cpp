#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int main() {
    pid_t pid;

    switch (pid = vfork()) {
        case -1: fprintf(stderr, "vfork() error\n");break;
        case 0: {
            cout<<"child process is about to close STDOUT_FILENO\n";
            close(STDOUT_FILENO);
            write(STDOUT_FILENO, "you should not see this\n", 24);
            break;
        }
        default: {
            cout<<"parent process is about to write to STDOUT_FILENO\n";
            write(STDOUT_FILENO, "you should see this\n", 20);
        }

    }
}