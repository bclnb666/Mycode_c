/***********************************************************
* File Name:    11exec.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月24日 星期二 16时55分09秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    pid_t pid;
    printf("Begin...\n");
    
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if(pid == 0)
    {
        execl("/usr/bin/ls","ls","-l",NULL);
        perror("exec()");
        exit(2);
    }
    
    wait(NULL);
    printf("End...\n");
    
    return 0;
}




