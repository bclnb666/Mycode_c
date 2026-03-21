/***********************************************************
* File Name:    mytouch03.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月17日 星期二 15时06分58秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    
    if(argc < 2)
    {
        fprintf(stderr,"用法：%s 文件1 文件2...\n",argv[0]);
        return 0;
    }
    for(int i = 1;i < argc;i++)
    {
        const char *pathhome = argv[i];
        int fd = open(pathhome,O_WRONLY | O_CREAT | O_EXCL,0666);
        if(fd == -1)
        {
            if(errno == EEXIST)
                continue;
            else
                perror(pathhome);
        }
        else
            close(fd);
    }   

    return 0;
}
