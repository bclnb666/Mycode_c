/***********************************************************
* File Name:    mycat03.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月17日 星期二 10时12分48秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

static void mycat(const char *pathhome)
{
    FILE *fp = NULL;
    char *buf[BUFSIZE];
    int ch = 0;
    fp = fopen(pathhome,"r");
    if(fp == NULL)
    {
        perror("fopen()");
        return -1;
    }
    ch = fread(buf,1,1,pathhome);
}



int main(int argc, char *argv[]) {
    
    

    return 0;
}
