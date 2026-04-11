/***********************************************************
* File Name:    05alarm.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月26日 星期四 16时37分14秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    
    signal(SIGALRM,sig_handler);
    alarm(1);
    while(1)
    {

    }

    return 0;
}
