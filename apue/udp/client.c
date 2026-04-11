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

#include "protocol.h"


#define SERVER_IP argv[1]
/*
把argv[1]当作ID
把argv[2]当作MSG
*/
int main(int argc, char *argv[])
{
    int sd = 0;//存储创建成功的套接字描述符
    struct data_st buf;//存储要发送的数据
    struct sockaddr_in server_addr;//存储对端的地址

    if(argc < 4)//判断命令行参数的个数是否少于3个
    {
        fprintf(stderr, "Usage : %s +IP + ID + MSG\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数的个数少于3个,结束程序,并且返回-1
    }

    //[1]创建套接字
    sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字
    if(sd == -1)//判断创建报式套接字是否失败
    {
        perror("socket()");//打印错误信息
        return -2;//由于创建报式套接字失败,结束程序,并且返回-2
    }

    //[2]发送数据包
    buf.id = atoi(argv[1]);//把argv[1]转化为整型的ID
    strncpy(buf.msg, argv[2], MSGSIZE);//把argv[2]拷贝到msg中
    server_addr.sin_family = AF_INET;//指定IPv4协议
    inet_aton(SERVER_IP, &server_addr.sin_addr);//转换对端IP地址
    //把点分十进制地址字符串转换为地址结构体
    server_addr.sin_port = htons(SERVER_PORT);//转换对端端口号
    //将本地字节序的端口号转换为网络字节序
    if(sendto(sd, &buf, sizeof(buf.id)+strlen(buf.msg)+1, \
    0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("sendto()");//打印错误信息
        close(sd);//关闭套接字文件
        return -3;//由于发送数据失败,结束程序,并且返回-3
    }

    //[3]关闭套接字
    close(sd);//关闭套接字文件

    return 0;
}


