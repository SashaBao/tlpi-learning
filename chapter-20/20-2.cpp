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
    cout<<"你要是能看到这行输出那就真的是奇了怪了"<<endl;

}