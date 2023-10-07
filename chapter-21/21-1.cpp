#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <iostream>
using namespace std;
sigjmp_buf buf; //����ȫ�� ��Ȼsiglongjmp�޷�ʹ��
void myabort() {
    sigset_t sigabrt;
    sigemptyset(&sigabrt);
    sigaddset(&sigabrt, SIGABRT);
    if (sigprocmask(SIG_UNBLOCK, &sigabrt, nullptr) < 0) {
        perror("sigprocmask()");
    }
    raise(SIGABRT);//abort�ɹ���Ȼ�Ͳ���ִ������Ĵ���
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
    // ���Ե�SIGABRT��������ʱ��
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
    // �����Ե����ͬ���Ͳ�������
    // ���ԷǱ�����ת
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