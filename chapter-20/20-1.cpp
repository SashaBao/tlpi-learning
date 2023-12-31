#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"
#include "tlpi_hdr.h"

static int sig_cnt[NSIG];
static volatile sig_atomic_t got_sigint = 0;

static void handler(int sig)
{
    if (sig == SIGINT)
        got_sigint = 1;
    else
        ++sig_cnt[sig];
}

int main(int argc, char *argv[])
{
    int n, num_secs;
    sigset_t pending_mask, blocking_mask, empty_mask;

    printf("%s: PID is %ld\n", argv[0], (long)getpid());
    struct sigaction newact, oldact;
    sigemptyset(&newact.sa_mask);
    newact.sa_handler = handler;
    newact.sa_flags = 0;
    for (n = 1; n < NSIG; ++n)
        sigaction(n, &newact, &oldact);

    if (argc > 1)
    {
        num_secs = getInt(argv[1], GN_GT_0, NULL);

        sigfillset(&blocking_mask);
        if (sigprocmask(SIG_SETMASK, &blocking_mask, NULL) == -1)
            errExit("sigprocmask");

        printf("%s: sleeping for %d seconds\n", argv[0], num_secs);
        sleep(num_secs);

        if (sigpending(&pending_mask) == -1)
            errExit("sigpending");

        printf("%s: pending singals are: \n", argv[0]);
        print_sigset(stdout, "\t\t", &pending_mask);

        sigemptyset(&empty_mask);
        if (sigprocmask(SIG_SETMASK, &empty_mask, NULL) == -1)
            errExit("sigprocmask");
    }

    while (!got_sigint)
        continue;

    for (n = 1; n < NSIG; ++n)
    {
        if (sig_cnt[n] != 0)
            printf("%s: signal %d caugnt %d time%s\n", argv[0], n,
                   sig_cnt[n], (sig_cnt[n] == 1) ? "" : "s");
    }

    exit(EXIT_SUCCESS);
}