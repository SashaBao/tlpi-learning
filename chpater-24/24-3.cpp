#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    pid_t pid;
    switch (pid = fork()) {
        case -1: fprintf(stderr, "fork() error\n");break;
        case 0:
            // signal(SIGABRT, SIG_DFL); 
            abort();
        default:;
    }
    return 0;
}