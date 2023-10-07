#include <signal.h>
#include <unistd.h>
#include <iostream>
using namespace std;

void handler(int sig)
{
    for (int i = 0; i < 99999999; ++i) {
        sleep(1);
    }
}

int main(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = SA_NODEFER;
    sigaction(SIGINT, act, nullptr);
    while (1);
}