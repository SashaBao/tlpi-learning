#include <signal.h>
#include <iostream>
using namespace std;

void handler(int sig)
{
    if (sig == SIGINT) {
        cout<<"gotcha!"<<endl;
    }
}

int main(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &act, nullptr);
    while (1);
}