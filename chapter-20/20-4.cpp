#include <signal.h>
#include <unistd.h>
using namespace std;
int mysiginterrupt(int sig, int flag) {
    struct sigaction act;
    sigaction(sig, NULL, &act);
    if (flag == 0) {
        act.sa_flags |= SA_RESTART;
    } else {
        act.sa_flags &= ~(SA_RESTART); //Çå³ýµôAS_RESTART
    }
    sigaction(sig, &act, NULL);

    return 0;
}
int main(int argc, char **argv) {

}