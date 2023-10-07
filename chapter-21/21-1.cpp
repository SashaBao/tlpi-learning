#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <iostream>
using namespace std;
sigjmp_buf buf; //必须全局 不然siglongjmp无法使用
void myabort() {
    sigset_t sigabrt;
    sigemptyset(&sigabrt);
    sigaddset(&sigabrt, SIGABRT);
    if (sigprocmask(SIG_UNBLOCK, &sigabrt, nullptr) < 0) {
        perror("sigprocmask()");
    }
    raise(SIGABRT);//abort成功自然就不会执行下面的代码
    struct sigaction oldact;
    sigaction(SIGABRT, nullptr, &oldact);
    oldact.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &oldact, nullptr);
    raise(SIGABRT);
}
void handler(int sig) {
    cout<<"SIGABRT has been caught\n";
    siglongjmp(buf, 1);
}
int main(int argc, char **argv) {
    // 测试当SIGABRT被阻塞的时候
    cout<<"when SIGABRT is ignored:\n";
    sigset_t sigabrt;
    sigemptyset(&sigabrt);
    sigaddset(&sigabrt, SIGABRT);
    if (sigprocmask(SIG_BLOCK, &sigabrt, nullptr) < 0) {
        perror("sigprocmask()");
    }
    cout<<"SIGABRT has successfully been ignored:\n";
    abort();
    cout<<"if abort succeeds, you shouldnt see the output.\n";
    // 被忽略的情况同理，就不测试了
    // 测试非本地跳转
    cout<<"when nonlocal:\n";
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigaction(SIGABRT, &act, nullptr);
    if (sigsetjmp(buf, 1) == 0) {
        cout<<"sigsetjmp has been called\n";
    } else {
        cout<<"jumped from SIGABRT handler\n";
    }
    myabort();
    cout<<"in this case, you should see the output.\n";
}