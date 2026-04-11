/***********************************************************
* File Name:    mydu.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月19日 星期四 14时40分32秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define BUFSIZE 1024

static long _mydu_blocks(const char *pathname)
{
    struct stat fs;//存储获取到的文件元信息
    long count = 0;//记录目录整体的块数
    struct dirent *entry = NULL;
    DIR *dp = NULL;
    char buf[BUFSIZE] = {0};


    if(stat(pathname,&fs) == -1)
    {
        perror("stat()");
        return -1;
    }
    if(!S_ISDIR(fs.st_mode))//判断是否不是目录文件
    {
        return fs.st_blocks;
    }

    //获取目录本身块数
    count = fs.st_blocks;
    dp = opendir(pathname);
    if(dp == NULL)
    {
        perror("opendir()");
        return -2;
    }
    
    while(1)
    {
        errno = 0;
        entry = readdir(dp);
        if(entry == NULL)
        {
            if(entry != 0)
            {
                closedir(dp);
                perror("readdir()");
                return -3;
            }
            break;
        }
        if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
            continue;
        memset(buf,0,BUFSIZE);
        strcpy(buf,pathname);
        strcat(buf,"/");
        strcat(buf,entry->d_name);
        count += _mydu_blocks(buf);
    }
    return count;
}

static long mydu(const char *pathname)
{
    return _mydu_blocks(pathname) / 2;
}


int main(int argc, char *argv[]) {
    
    if(argc < 2)
    {
        fprintf(stderr,"Usage : %s + filename\n",argv[0]);
        return -1;
    }
    printf("%ldK\t%s\n",mydu(argv[1]),argv[1]);
    return 0;
}






