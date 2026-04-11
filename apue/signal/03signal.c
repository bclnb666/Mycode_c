/***********************************************************
* File Name:    01sihnal_2.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月26日 星期四 11时47分46秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void handler(int none)
{
    write(1,"!",1);
}

int main(int argc, char *argv[]) {
    //sighandler_t old;

    int i = 0;

    if((signal(SIGINT,handler)) == SIG_ERR)
    {
        perror("signal()");
        return -1;
    }
    while(1)
    {

        write(1,"~",1);
        sleep(1);
        i++;
        if(i == 10)
            signal(SIGINT,SIG_DFL);
    }
    
    return 0;
}
