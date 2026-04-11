/***********************************************************
* File Name:    14anytimer.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月27日 星期五 15时15分05秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_handler(int none)
{
    write(1,"!",1);
}

int main(int argc, char *argv[]) {
    
    struct sigaction act;
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    sigaction(SIGALRM,&act,NULL);
    
    alarm(1);
    alarm(10);
    alarm(5);
    while(1)
    {
        write(1,"*",1);
        sleep(1);
    }
    return 0;
}
