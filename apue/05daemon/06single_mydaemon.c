/**********************************************************
* File Name:    01relationship.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月25日 星期三 09时46分02秒
***********************************************************/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>


#define BUFSIZE 128
#define LOCKFILE "/var/run/mydaemon.pid"


static int Mydaemon(void)
{
    pid_t pid;
    int fd = 0;

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        return -1;
    }
    if(pid > 0)
        _exit(0);
    if(setsid() == -1)
    {
        perror("setsid()");
        return -2;
    }
    umask(0);
    if(chdir("/") == -1)
    {
        perror("chdir()");
        return -3;
    }
    fd = open("/dev/null",O_RDWR);
    if(fd < 0)
    {
        perror("open()");
        return -4;
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd > 2)
        close(fd);

    return 0;
}

static int process_already_running(void)
{
    int fd = 0;
    char buf[BUFSIZE] = {0};

    fd = open(LOCKFILE,O_WRONLY | O_CREAT,0666);
    if(fd < 0)
    {
        perror("open()");
        return -1;
    }
    if(flock(fd,LOCK_EX | LOCK_NB) == -1)
    {
        perror("flock()");
        fclose(fd);
        return -2;
    }
    if(ftruncate(fd,0) == -1)
    {
        perror("ftruncate()");
        fclose(fd);
        return -3;
    }
    
    sprintf(buf,"%d",getpid());
    write(fd,buf,strlen(buf));

    return 0;
}

int main(void)
{
    FILE *fp = NULL;//fp指针指向打开的"/tmp/out"文件的文件流
    int ret = 0;//ret存储错误码
    time_t tm;//存储当前时间的时间戳
    struct tm *time_st = NULL;//time_st指针指向格式化时间的空间
    char buf[BUFSIZE] = {0};//buf数组用来存储格式化时间的字符串
    
    if(daemon(0,0) < 0)
    {
        perror("daemon()");
        goto ERR_1;
    }

    if(process_already_running() < 0)
    {
        fprintf(stderr,"process_already_running() is failed!\n");

        goto ERR_1;
    }

    fp = fopen("/tmp/out", "w");//以w的形式打开目标文件
    if(fp == NULL)//判断打开文件是否失败
    {
        perror("fopen()");//打印错误信息
        ret = -1;//设置错误码为-1
        goto ERR_1;//跳转到ERR_1的标志
    }

    while(1)//死循环
    {
        if(time(&tm) == (time_t)-1)//判断获取当前时间的时间戳是否失败
        {
            perror("time()");//打印错误信息
            ret = -2;//设置错误码为-2
            goto ERR_2;//跳转到ERR_2的标志
        }
        if((time_st = localtime(&tm)) == NULL)//判断把时间戳转换成格式化时间是否失败
        {
            perror("localtime()");//打印错误信息
            ret = -3;//设置错误码为-3
            goto ERR_2;//跳转到ERR_2的标志
        }
        //memset();//清空脏数据
        strftime(buf, BUFSIZE, "%Y年%m月%d日 %H:%M:%S\n", time_st);
        //把格式化时间转换成格式化时间的字符串
        fputs(buf, fp);//把buf存储的字符串写入到fp指针指向的文件流中
        fflush(NULL);//刷新缓冲区
        sleep(1);
    }

ERR_2:
    fclose(fp);//关闭目标文件的文件流
ERR_1:
    return ret;
}

