#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;
void x() {
    if (setjmp(env) == 0) {
        printf("x() first call setjump!\n");
    } else {
        printf("y() call longjmp successfully!\n");
    }
}

void y() {
    printf("y() starts to call longjmp\n");
    longjmp(env, 1);
}
int main(int argc, char **argv) {
    x();
    printf("x() returns\n");
    y();
    return 0;
}