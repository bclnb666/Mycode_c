/***********************************************************
* File Name:    03fork.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月24日 星期二 10时14分17秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    
    int num = 10;
    pid_t pid;
    printf("调用fork之前，只有父进程PID ：%d,num = %d\n",getpid(),num);
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if(pid == 0)
    {
        sleep(30);
        num += 5;
        printf("I am Child Process!(PID:%d,PPID:%d) num = %d\n",getpid(),getppid(),num);
    }
    else
    {
        num -= 5;
        printf("I am Parent Process!(PID:%d,CPPID:%d) num = %d\n",getpid(),pid,num);
        //wait(NULL);
       
    }
    printf("PID:%d Game Over num:%d\n",getpid(),num);

    exit(0);
}



