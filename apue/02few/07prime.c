/***********************************************************
* File Name:    07prime.c
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

#define MIN 100
#define MAX 300
#define NUM (MAX - MIN + 1)


static int is_prime(int num)
{
    int i = 0;
    sleep(1);
    if(num <= 1)
        return 0;

    if(num == 2 | num == 3)
    {
        return 1;
    }
    for(i = 2;i < num / i; i++)
    {
        if(num % i == 0)
            return 0;
    }
    return -1;
}

int main(int argc, char *argv[]) {

    int i = 0;
    pid_t pid;

    for(i = MIN; i <= MAX; i++)
    {
        pid  = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        if(pid == 0)
        {
            if(is_prime(i))
                printf("%d is prime!\n",i);
            exit(0);
        }
    }
    
    for(i = 0; i < NUM; i++)
    {
        wait(NULL);
    }

    return 0;
}
