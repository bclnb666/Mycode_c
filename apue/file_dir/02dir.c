/***********************************************************
* File Name:    02dir.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月19日 星期四 11时36分02秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    struct stat fs;
    DIR *dp = NULL;
    struct dirent *entry = NULL;//指向目录项结构

    int ret = 0;

    if(argc < 2)
    {
        fprintf(stderr,"Usage : %s + filename\n",argv[0]);
        ret = -1;
        goto ERR_1;
    }
    if(stat(argv[1],&fs) == -1)
    {
        perror("stat()");
        ret = -2;
        goto ERR_1;
    }
    //判断是否不是目录文件
    if(!S_ISDIR(fs.st_mode))
    {
        printf("%s Not a Directory!\n",argv[1]);
        ret = -3;
        goto ERR_1;
    }
    
    dp = opendir(argv[1]);
    if(dp == NULL)
    {
        perror("opendir()");
        ret = -4;
        goto ERR_1;
    }
    while(1)//循环读取目录项结构
    {
        errno = 0;
        entry = readdir(dp);
        if(entry == NULL)
        {
            if(errno != 0)
            {
                perror("readdir()");
                ret = -5;
                goto ERR_2;
            }
            break;
        }
        printf("%ld-%s\n",entry->d_ino,entry->d_name);
    }

ERR_2:
    closedir(dp);
ERR_1:
    return ret;
}








