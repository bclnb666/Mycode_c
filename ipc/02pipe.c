/***********************************************************
* File Name:    02pipe.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月30日 星期一 15时16分50秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFSIZE 32

int main(int argc, char *argv[]) {
    
    int pfd[2];

    int ret = 0;
    pid_t pid;
    char buf[BUFFSIZE] = {0};

    if(pipe(pfd) == -1)
    {
        perror("pipe()");
        ret = -1;
        goto ERR_1;
    }

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        ret = -2;
        goto ERR_2;
    }
    if(pid == 0)
    {   
        close(pfd[0]);
        write(pfd[1],"Hello World!",12);
        close(pfd[1]);
        exit(0);
    }
    close(pfd[1]);
    wait(NULL);
    read(pfd[0],buf,BUFFSIZE);
    printf("parent : %s\n",buf);
    close(pfd[0]);

    return 0;

ERR_2:
    close(pfd[1]);
    close(pfd[0]);
ERR_1:
    return ret;
}






