#include <wait.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
int main()
{
    int status;
    pid_t pid;
    switch (pid = fork()) {
        case 0 : exit(-1);break;
        default : {
            wait(&status);
            cout<<WEXITSTATUS(status);
            break;
        }
    }

    return 0;
}