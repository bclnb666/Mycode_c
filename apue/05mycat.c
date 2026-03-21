/***********************************************************
* File Name:    mycat04.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月17日 星期二 16时12分28秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define DUP2

#define BUFFER_SIZE 4096

static int mycat(int fd)
{
    char buf[BUFFER_SIZE] = {0};
    int count = 0;

    while(1)
    {
        memset(buf,0,sizeof(buf));
        count = read(fd,buf,sizeof(buf));
        if(count == 0)
            break;
        else if(count < 0)
        {
            perror("read()");
            return -1;
        }
        write(1,buf,count);
    }
}



int main(int argc, char *argv[]) {
    

    if(argc < 2)
    {
        fprintf(stderr,"用法 %s 文件1 文件2...\n",argv[0]);
        return -1;
    }
    for(int i = 1;i < argc;i++)
    {
        int fd = open(argv[i],O_RDONLY);
        int fdo = 0;
        if(fd < 0)
        {
            perror("open()");
            return -2;
        }
        fdo = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0666);
        if(fdo < 0)
        {
            perror("open()");
            return -3;
        }
#if define(DUP)
        close(1);
        dup(fdo);
        
#elif define(DUP2)
        dup2(fdo,1);

#endif

        mycat(fd);
        close(fdo);
        close(fd);
    }
   
    return 0;
}















