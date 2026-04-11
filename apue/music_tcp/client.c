/***********************************************************
* File Name:    client.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月08日 星期三 11时20分07秒
***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include "protocol.h"

int main(int argc, char *argv[])
{
    int sd = 0;//存储创建成功的套接字描述符
    struct data_st buf;//存储接收到的数据
    struct sockaddr_in my_addr;//存储本地的地址
    struct ip_mreqn imr;//存储组播选项的结构
    int pd[2];//存储匿名管道的文件描述符
    pid_t pid;//存储子进程的标识
    ssize_t cnt = 0;//存储成功接收到数据的字节数

    //[1]创建匿名管道(用于父子进程通信)
    if(pipe(pd) == -1)//判断创建匿名管道是否失败
    {
        perror("pipe()");//打印错误信息
        return -1;//由于创建管道失败,结束程序,并且返回-1
    }

    //[2]创建子进程
    pid = fork();//创建子进程
    if(pid == -1)//判断创建子进程是否失败
    {
        perror("fork()");//打印错误信息
        close(pd[0]);//关闭管道读端
        close(pd[1]);//关闭管道写端
        return -2;//由于创建子进程失败,结束程序,并且返回-2
    }

    if(pid == 0)//子进程负责进程替换,进行播放
    {
        close(pd[1]);//子进程关闭管道写端
        dup2(pd[0], 0);//将子进程的标准输入重定向到管道读端
        close(pd[0]);//关闭原有的管道读端描述符

        //进行进程替换,调用 mplayer 播放器进行播放
        //加入 -cache 8192 参数，为线性流提供8MB的内存缓存，解决FLAC等格式需要seek backward的问题
        // "-"代表 mplayer 会从标准输入(也就是管道读端)读取音频流数据
        execlp("mplayer", "mplayer", "-cache", "8192", "-", NULL);

        perror("execlp()");//如果execlp能执行到这里说明替换失败
        exit(1);//终止子进程
    }

    //=== 以下为父进程代码 ===
    close(pd[0]);//父进程关闭管道的读端

    //[3]创建套接字
    sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字
    if(sd == -1)//判断创建报式套接字是否失败
    {
        perror("socket()");//打印错误信息
        close(pd[1]);//关闭管道写端
        return -3;//由于创建报式套接字失败,结束程序,并且返回-3
    }

    //[4]将本地地址与套接字绑定
    my_addr.sin_family = AF_INET;//指定IPv4协议
    inet_aton(LOCAL_IP, &my_addr.sin_addr);//转换本地IP
    my_addr.sin_port = htons(RECV_PORT);//转换本地PORT
    if(bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
    {
        perror("bind()");//打印错误信息
        close(sd);//关闭套接字文件
        close(pd[1]);//关闭管道写端
        return -4;//由于将本地地址与套接字绑定失败,结束程序,并且返回-4
    }

    //[5]加入到多播组
    inet_aton(MULTICAST_IP, &imr.imr_multiaddr);//转换组播地址
    inet_aton(LOCAL_IP, &imr.imr_address);//转换本地地址
    imr.imr_ifindex = if_nametoindex(NETCARD_NAME);//转换网卡号
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, \
                  &imr, sizeof(imr)) == -1)//判断加入多播组是否失败
    {
        perror("setsockopt()");//打印错误信息
        close(sd);//关闭套接字文件
        close(pd[1]);//关闭管道写端
        return -5;//由于加入多播组失败,结束程序,并且返回-5
    }

    //[6]父进程从多播组里面读出数据，写入管道
    while(1)
    {
        cnt = recvfrom(sd, buf.msg, MSGSIZE, 0, NULL, NULL);//不需要对端地址，填NULL
        if(cnt == -1)//判断接收数据是否失败
        {
            perror("recvfrom()");//打印错误信息
            break;
        }

        //将父进程读到的多播数据写入匿名管道，传递给子进程的播放器
        write(pd[1], buf.msg, cnt);
    }

    //[7]关闭套接字和文件描述符
    close(sd);//关闭套接字文件
    close(pd[1]);//关闭管道写端
    return 0;
}
