/***********************************************************
* File Name:    08sigmask.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月27日 星期五 11时04分30秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler(int none)
{
    write(1,"@",1);

}

int main(int argc, char *argv[]) {
    
    int i = 0, j = 0;
    sigset_t old,new;
    sigemptyset(&new);
    sigaddset(&new,SIGINT);

    signal(SIGINT,sig_handler);
    for(int i = 0;i < 10;i++)
    {
        sigprocmask(SIG_BLOCK,&new,&old);
        for(int j = 0;j < 5;j++)
        {
            write(1,"#",1);
            sleep(1);
        }
        write(1,"\n",1);
        sigprocmask(SIG_SETMASK,&old,NULL);
    }

    return 0;
}










