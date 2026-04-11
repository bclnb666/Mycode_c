/***********************************************************
 * File Name:    09cp_on_write.c
 * Author:       Amon
 * mail:         bamon1026@gmail.com
 * Created Time: 2026年03月24日 星期二 14时58分00秒
 ***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



int main(int argc, char *argv[]) {

    int i = 10010;
    pid_t pid;
    pid  = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if(pid == 0)
    {
        printf("child.i = %p i = %d\n",&i,i);
        i = 10086;
        printf("child.i = %p i = %d\n",&i,i);
        exit(0);
    }
    else
    {
        wait(NULL);
        printf("parent.i = %p i = %d\n",&i,i);
        i = 12306;
        printf("parent.i = %p i = %d\n",&i,i);
    }

    return 0;
}









