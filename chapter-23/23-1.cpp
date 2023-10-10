#include <sys/time.h>
using namespace std;
unsigned int myalarm(unsigned int seconds) {
    struct itimerval new_value， old_value;
    // if (seconds == 0) { 无需特判
    //     new_value.it_value = {0, 0};
    //     setitimer(ITIMER_REAL, &new_value, nullptr);
    //     return 0;
    // }
    new_value.it_interval = {0, 0};
    new_value.it_value = {seconds, 0};

	setitimer(ITIMER_REAL, &new_value, &old_value); //ITIMER_REAL会发送SIGALRM信号
	return old_value.it_value.tv_sec;
    
}
