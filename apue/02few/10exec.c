/***********************************************************
* File Name:    10exec.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月24日 星期二 16时55分09秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    printf("Begin...\n");
    
    execl("/usr/bin/ls","ls","-l",NULL);

    printf("End...\n");
    
    return 0;
}
