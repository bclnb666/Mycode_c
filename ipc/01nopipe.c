/***********************************************************
* File Name:    01nopipe.c
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
    
    int fd = 0;
    int ret = 0;
    pid_t pid;
    char buf[BUFFSIZE] = {0};

    fd = open("./hello",O_RDWR | O_CREAT | O_TRUNC,0666);
    if(fd < 0)
    {
        perror("open()");
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
        write(fd,"Hello World!",12);
        close(fd);
        exit(0);
    }
    wait(NULL);
    lseek(fd,SEEK_SET,0);
    read(fd,buf,BUFFSIZE);
    printf("parent : %s\n",buf);

ERR_2:
    close(fd);
ERR_1:
    return ret;
}






