/**********************************************************
* File Name:    03test.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月30日 星期一 15时16分50秒
***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define BUFSIZE 128


int main(int argc, char * argv[])
{
    int i = 0;
    int ret = 0;//存储错误码
    int fd = 0;//用来存储打开argv[1]文件的文件描述符
    int pfd[2];//用来存储pipe的读端和写端的文件描述符
    pid_t pid;//用来存储子进程的标识

    if(pipe(pfd) == -1)//判断创建pipe是否失败
    {
        perror("pipe()");//打印错误信息
        ret = -1;//
        goto ERR_1;//由于创建pipe失败,跳转到ERR_2的位置
    }


    for(;i < 2;i++)
    {
        pid = fork();
        if(pid < 0)//判断创建子进程是否失败
        {
            perror("fork()");//打印错误信息
            ret = -2;//存储-4错误码
            goto ERR_2;//由于创建子进程失败,跳转到ERR_3的位置
        }
        if(pid == 0)
            break;
    }

    if(i == 0)//子进程的操作
    {
        close(pfd[0]);
        dup2(pfd[1],1);
        execl("/usr/bin/ps","ps","ajx",NULL);
        close(pfd[1]);
        exit(0);
    }
    if(i == 1)
    {
        close(pfd[1]);
        dup2(pfd[0],0);
        execl("/usr/bin/grep","grep","systemd",NULL);
        close(pfd[0]);
        exit(0);
    }
    //父进程的操作
    close(pfd[0]);//父进程关闭pipe的读端
    close(pfd[1]);//父进程关闭pipe的写端
    for(i = 0;i < 2;i++)
        wait(NULL);//等待子进程结束
    return 0;

ERR_2 :
    close(pfd[1]);//关闭pipe的写端
    close(pfd[0]);//关闭pipe的读端
ERR_1 :
    return ret;
}

