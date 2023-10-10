#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

static void handler(int sig) {  }

int main(int argc, const char *argv[]){

    // if(argc != 3 || strcmp(argv[1], "--help") == 0)
    //     usageErr("%s secs nanosecs", argv[0]);

    struct sigaction siga;
    siga.sa_handler = handler;
    siga.sa_flags   = 0;
    sigemptyset(&siga.sa_mask);

    sigaction(SIGINT, &siga, NULL);

    struct timeval st, dt;

    // if(gettimeofday(&st, NULL) == -1) errExit("gettimeofday");
    gettimeofday(&st, NULL);

    struct timespec request, remain;

    request.tv_sec = 60;
    request.tv_nsec= 0;

    for(;;){
        int s = nanosleep(&request, &remain);
        // if(s == -1 && errno != EINTR) errExit("nanosleep");

        // if(gettimeofday(&dt, NULL) == -1) errExit("gettimeofday");
        gettimeofday(&dt, NULL);

        printf("Slept for: %9.6f secs\n", dt.tv_sec - st.tv_sec + (dt.tv_usec - st.tv_usec) / 1000000.0 );

        if(s == 0) break;

        printf("Remaining: %2ld.%09ld\n", (long) remain.tv_sec, remain.tv_nsec);

        request = remain;

    }
    
    return 0;
}