/***********************************************************
* File Name:    01relationship.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月25日 星期三 09时46分02秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    
    pid_t pid;
    printf("---------parent---------\n");
    printf("PID:%d PPID:%d PGID:%d SID:%d\n",\
            getpid(),getppid(),getpgid(0),getsid(0));
    printf("------------------------\n");

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)
    {
        if(setpgid()(getpid(),getpid()) != 0)
        {
            perror("setpgid()");
        }
        printf("---------child---------\n");
        printf("PID:%d PPID:%d PGID:%d SID:%d\n",\
                getpid(),getppid(),getpgid(0),getsid(0));
        printf("------------------------\n");
        exit(0);
    }

    wait(NULL);
    
    return 0;
}


