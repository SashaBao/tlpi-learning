#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZE 100
#define MAXSIZE 100

extern int optind, opterr, optopt;
extern char *optarg;

int main(int argc, char *argv[]) {
    int opt, fd, hasOpta = 0;
    char *pstr, buf[BUFSIZE]; 
    int teeFds[MAXSIZE], teeFdsId = 0;

    memset(teeFds, 0, sizeof teeFds);
    ssize_t numRead, numWritten;
    if ((opt = getopt(argc, argv, "a")) != -1) {
        while (argv[optind]) { //opting是argv下一个要处理元素的坐标
            pstr = argv[optind];
            teeFds[teeFdsId++] = open(pstr, O_WRONLY | O_APPEND | O_CREAT);
            ++optind;
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            teeFds[teeFdsId++] = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC);
        }
    }
    while ((numRead = read(STDIN_FILENO, buf, BUFSIZE)) != -1) {
        numWritten = write(STDOUT_FILENO, buf, numRead);
        for (int i = 0; i < teeFdsId; ++i) {
            numWritten = write(teeFds[i], buf, numRead);
        }
    }
    exit(0);
}