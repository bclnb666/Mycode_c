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

#define BUFFER_SIZE 4096

static int mycp(int fd ,int fd2)
{
    char buf[BUFFER_SIZE];
    int count = 0;

    while(1)
    {
        memset(buf,0,sizeof(buf));
        count = pread(fd,buf,sizeof(buf),0);
        if(count == 0)
            break;
        else if(count < 0)
        {
            perror("read()");
            return -1;
        }
        pwrite(fd2,buf,count,count);   
    }
}



int main(int argc, char *argv[]) {
    

    if(argc < 2)
    {
        fprintf(stderr,"用法: %s 文件1 文件2\n",argv[0]);
        return -1;
    }
        int fd = open(argv[1],O_RDONLY);
        int fd2 = open(argv[2],O_WRONLY | O_CREAT | O_EXCL,0666);
        if(fd < 0 && fd2 < 0)
        {
            perror("open()");
            return -2;
        }

        mycp(fd,fd2);

        close(fd);
        close(fd2);
    
   
    return 0;
}















