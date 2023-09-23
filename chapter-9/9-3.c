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

int myinitgroups(const char *user, gid_t group) {
    setgrent();
    struct group *tmp;
    gid_t groups[30];//if in cpp should use vector because of dynamic
    int cnt = 0;
    while ((tmp = getgrent()) != NULL) {
        while (*tmp->gr_mem != NULL ) {
            if (strcmp(*tmp->gr_mem, user) == 0) {
                groups[cnt++] = tmp->gr_gid;
                break;
            }
            ++tmp->gr_mem;//not *tmp->gr_mem
        }
    }
    endgrent();
    groups[cnt++] = group;
    return setgroups(cnt, groups);
    
}
int main(int argc, char **argv) {
    if (myinitgroups("mayuri", 666) == -1) {
        perror("setgroup()");//setgroups sets errno
        exit(-1);
    }
    
    gid_t groups[30];
    int group_num;
    group_num = getgroups(30, groups);

    for (int i = 0; i < group_num; ++i) {
        printf("%ld ", (long) groups[i]);
    }
    printf("\n");
    return 0;
}