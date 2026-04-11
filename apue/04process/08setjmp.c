/***********************************************************
* File Name:    08setjmp.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 15时38分25秒
***********************************************************/
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

jmp_buf env;
int *p = NULL;
FILE *fp = NULL;
int fd;

void make_money()
{
    p = malloc(4);
    if(p == NULL)
        longjmp(env,3);
    fp = fopen("./car","r");
    if(fp == NULL)
        longjmp(env,2);
    fd = open("./huangjin",O_RDONLY);
    longjmp(env,1);
}

int main(int argc, char *argv[]) {

    switch(setjmp(env))
    {
        case 0: printf("我杨你阿！\n");
                make_money();
                printf("666!\n");
                break;
        case 1: printf("算了吧！\n");fclose(fp);fp = NULL;
        case 2: printf("细细水坝！\n");free(p);p = NULL;
        case 3: puts("咸阳好自己把！\n");break;
    }
    

    return 0;
}
