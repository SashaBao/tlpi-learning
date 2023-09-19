#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_ALLOCS 1000000

int main(int argc, char **argv) {
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
    printf("\n");
    numAllocs = atoi(argv[1]);
    blockSize = atoi(argv[2]);
    freeStep = (argc > 3) ? atoi(argv[3]) : 1;
    freeMin = (argc > 4) ? atoi(argv[4]) : 1;
    freeMax = (argc > 5) ? atoi(argv[5]) : numAllocs;

    printf("INitial program break:      %10p\n", sbrk(0));
    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; ++j) {
        if ((ptr[j] = malloc(blockSize)) == NULL) {
            fprintf(stderr, "malloc error");
        }
        printf("current program break:      %10p\n", sbrk(0));
    }
    printf("Freeing blcoks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j+= freeStep) {
        free(ptr[j]);
    }
    printf("After free(), program break is: %10p\n", sbrk(0));
    return 0;
}