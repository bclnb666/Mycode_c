/***********************************************************
* File Name:    09setrlimit.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 17时06分32秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
int main(int argc, char *argv[]) {

    struct rlimit limit;
    
    if(getrlimit(RLIMIT_STACK,&limit) != 0)
    {
        perror("getrlimit()");
        return -1;
    }
    printf("Stack limit: %ldKB\n",limit.rlim_cur >> 10);
    return 0;
}
