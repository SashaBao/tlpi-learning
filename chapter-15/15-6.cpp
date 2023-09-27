#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <sys/ioctl.h>
#include <linux/fs.h>

using namespace std;
int main(int argc, char *argv[]) {
    // int x;
    // while (x = getopt(argc, argv, "") != -1) {//只能解析出选项
    //     switch (x) {
    //         case '+':{

    //         }
    //     }
    // }
    int fd = open(argv[argc - 1], O_RDWR);
    for (int i = optind; i < argc; ++i) {
        int attr;
        if (ioctl(fd, FS_IOC_GETFLAGS, &attr) < 0) {
            perror("ioctl()");
            return -1;
        }
        switch (argv[i][0]) {
            case '+' : {
                for (int j = 1; argv[i][j]; ++j) {
                    switch (argv[i][j]) {
                        case 'a': attr |= FS_APPEND_FL;break;
                        case 'A': attr |= FS_NOATIME_FL;break;
                        case 'c': attr |= FS_COMPR_FL;break;
                        case 'D': attr |= FS_DIRSYNC_FL;break;
                        case 'i': attr |= FS_IMMUTABLE_FL;break;
                        case 'j': attr |= FS_JOURNAL_DATA_FL;break;
                        case 'd': attr |= FS_NODUMP_FL;break;
                        case 't': attr |= FS_NOTAIL_FL;break;
                        case 's': attr |= FS_SECRM_FL;break;
                        case 'S': attr |= FS_SYNC_FL;break;
                        case 'T': attr |= FS_TOPDIR_FL;break;
                        case 'u': attr |= FS_UNRM_FL;break;
                    }
                }
            }
            case '-': {
                for (int j = 1; argv[i][j]; ++j) {
                    switch (argv[i][j]) {
                        case 'a': attr &= ~FS_APPEND_FL;break;
                        case 'A': attr &= ~FS_NOATIME_FL;break;
                        case 'c': attr &= ~FS_COMPR_FL;break;
                        case 'D': attr &= ~FS_DIRSYNC_FL;break;
                        case 'i': attr &= ~FS_IMMUTABLE_FL;break;
                        case 'j': attr &= ~FS_JOURNAL_DATA_FL;break;
                        case 'd': attr &= ~FS_NODUMP_FL;break;
                        case 't': attr &= ~FS_NOTAIL_FL;break;
                        case 's': attr &= ~FS_SECRM_FL;break;
                        case 'S': attr &= ~FS_SYNC_FL;break;
                        case 'T': attr &= ~FS_TOPDIR_FL;break;
                        case 'u': attr &= ~FS_UNRM_FL;break;
                    }
                }
            }
            case '=': {
                for (int j = 1; argv[i][j]; ++j) {
                    switch (argv[i][j]) {
                        case 'a': attr = FS_APPEND_FL;break;
                        case 'A': attr = FS_NOATIME_FL;break;
                        case 'c': attr = FS_COMPR_FL;break;
                        case 'D': attr = FS_DIRSYNC_FL;break;
                        case 'i': attr = FS_IMMUTABLE_FL;break;
                        case 'j': attr = FS_JOURNAL_DATA_FL;break;
                        case 'd': attr = FS_NODUMP_FL;break;
                        case 't': attr = FS_NOTAIL_FL;break;
                        case 's': attr = FS_SECRM_FL;break;
                        case 'S': attr = FS_SYNC_FL;break;
                        case 'T': attr = FS_TOPDIR_FL;break;
                        case 'u': attr = FS_UNRM_FL;break;
                    }
                }
            }
        }
        if (ioctl(fd, FS_IOC_SETFLAGS, &attr) < 0) {
            perror("ioctl()");
            return -1;
        }    
    }


}