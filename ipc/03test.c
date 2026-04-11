/***********************************************************
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

static int cpfile(int rfd, int wfd)
{
    char buf[BUFSIZE] = {0};//用来存储读取出的数据
    int count = 0;//存储成功读取到的字节数

    while(1)
    {
        memset(buf, 0, BUFSIZE);//清空脏数据
        count = read(rfd, buf, BUFSIZE);//从rfd文件中读取数据
        if(count == -1)//判断读取数据是否失败
        {
            perror("read()");//打印错误信息
            return -1;//由于读取错误,结束函数,并且返回-1
        }
        if(count == 0)//判断是否读取到了文件结尾
            break;//跳出死循环
        write(wfd, buf, count);//把buf中的数据写到wfd文件中
    }

    return 0;
}

int main(int argc, char * argv[])
{
    int ret = 0;//存储错误码
    int fd = 0;//用来存储打开argv[1]文件的文件描述符
    int pfd[2];//用来存储pipe的读端和写端的文件描述符
    pid_t pid;//用来存储子进程的标识

    if(argc < 2)//判断命令行参数的个数是否少于2个
    {
        fprintf(stderr, "Usage : %s + filename\n", argv[0]);//打印使用说明
        ret = -1;//存储-1错误码
        goto ERR_1;//由于命令行参数的个数少于2个,跳转到ERR_1的位置
    }

    fd = open(argv[1], O_RDONLY);//打开argv[1]文件
    if(fd < 0)//判断打开argv[1]文件是否失败
    {
        perror("open()");//打印错误信息
        ret = -2;//存储-2错误码
        goto ERR_1;//由于打开argv[1]文件失败,跳转到ERR_1的位置
    }

    if(pipe(pfd) == -1)//判断创建pipe是否失败
    {
        perror("pipe()");//打印错误信息
        ret = -3;//存储-3错误码
        goto ERR_2;//由于创建pipe失败,跳转到ERR_2的位置
    }

    pid = fork();//创建子进程
    if(pid < 0)//判断创建子进程是否失败
    {
        perror("fork()");//打印错误信息
        ret = -4;//存储-4错误码
        goto ERR_3;//由于创建子进程失败,跳转到ERR_3的位置
    }
    if(pid == 0)//子进程的操作
    {
        close(pfd[1]);//子进程关闭pipe的写端
        cpfile(pfd[0], 1);//从pipe的读端读数据,写到1号文件描述符中
        close(pfd[0]);//子进程关闭pipe的读端
        exit(0);//终止子进程
    }
    //父进程的操作
    close(pfd[0]);//父进程关闭pipe的读端
    cpfile(fd, pfd[1]);//从argv[1]文件中读数据,写到pipe的写端
    close(pfd[1]);//父进程关闭pipe的写端
    wait(NULL);//等待子进程结束

    close(fd);//关闭argv[1]文件
    return 0;

ERR_3 :
    close(pfd[1]);//关闭pipe的写端
    close(pfd[0]);//关闭pipe的读端
ERR_2 :
    close(fd);//关闭argv[1]文件
ERR_1 :
    return ret;
}

