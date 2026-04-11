/***********************************************************
* File Name:    03env.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 11时39分45秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[]) {

    printf("argc = %d\n",argc);

    for(int i = 0; i < argc; i++)
    {
        puts(argv[i]);
    }
    printf("\n");


    for(int i = 0; environ[i] != NULL; i++)
    {
        puts(environ[i]);
    }
    printf("\n");
    return 0;
}
