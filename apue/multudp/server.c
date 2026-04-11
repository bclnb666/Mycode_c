/***********************************************************
* File Name:    server.c
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

#include "protocol.h"

int main(void)
{
    int sd = 0;//存储创建成功的套接字描述符
    struct data_st buf;//存储接收到的数据
    struct sockaddr_in my_addr;//存储本地的地址
    struct sockaddr_in remote_addr;//存储对端的地址
    socklen_t remote_addr_len;//存储对端地址的长度(必须要有初始化)
    int cnt = 0;//存储成功接收到数据的字节数

    //[1]创建套接字
    sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字
    if(sd == -1)//判断创建报式套接字是否失败
    {
        perror("socket()");//打印错误信息
        return -1;//由于创建报式套接字失败,结束程序,并且返回-1
    }

    //[2]将本地地址与套接字绑定
    my_addr.sin_family = AF_INET;//指定IPv4协议
    inet_aton(SERVER_IP, &my_addr.sin_addr);//转换本地IP
    //把点分十进制地址字符串转换为地址结构体
    my_addr.sin_port = htons(SERVER_PORT);//转换本地PORT
    //将本地字节序的端口号转换为网络字节序
    if(bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
    {
        perror("bind()");//打印错误信息
        close(sd);//关闭套接字文件
        return -2;//由于将本地地址与套接字绑定失败,结束程序,并且返回-2
    }

    //[3]接收数据
    //对端地址的长度必须要有初始化,否则影响接收
    remote_addr_len = sizeof(remote_addr);//初始化对端地址的长度
    while(1)
    {
        cnt = recvfrom(sd, &buf, sizeof(buf), 0, \
        (struct sockaddr *)&remote_addr, &remote_addr_len);
        if(cnt == -1)//判断接收数据是否失败
        {
            perror("recvfrom()");//打印错误信息
            close(sd);//关闭套接字文件
            return -3;//由于接收数据失败,结束程序,并且返回-3
        }
        printf("\n**********MSG**********\n");
        printf("Remote IP : %s\n", inet_ntoa(remote_addr.sin_addr));
        printf("Remote PORT : %d\n", ntohs(remote_addr.sin_port));
        printf("ID : %d\n", buf.id);
        printf("MSG : %s\n", buf.msg);
        printf("***********************\n");
    }

    //[4]关闭套接字
    close(sd);//关闭套接字文件

    return 0;
}

