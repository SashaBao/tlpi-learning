#include <signal.h>
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    for (int i = 1; i < NSIG; ++i) {
        sigaction(i, &act, nullptr);
    }
    pause();
    cout<<"��Ҫ���ܿ�����������Ǿ���������˹���"<<endl;

}