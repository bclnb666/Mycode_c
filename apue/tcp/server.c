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
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "protocol.h"

int main(void)
{
    int tcp_socket;//存储创建成功的套接字描述符
    int new_socket;
    struct sockaddr_in laddr;//存储本地的地址
    pid_t pid;
    int cnt = 0;//存储成功接收到数据的字节数

    //[1]创建套接字
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);//创建流式套接字
    if(tcp_socket == -1)
    {
        perror("socket()");
        return -1;
    }

    //[2]将本地地址与套接字绑定
    laddr.sin_family = AF_INET;//指定IPv4协议
    //inet_aton("0.0.0.0", &laddr.sin_addr);//转换本地IP(也可以使用宏)
    laddr.sin_addr.s_addr = INADDR_ANY;
    laddr.sin_port = htons(SERVER_PORT);//转换本地PORT

    if(bind(tcp_socket, (struct sockaddr *)&laddr, sizeof(laddr)) == -1)
    {
        perror("bind()");
        close(tcp_socket);
        return -2;
    }

    //[3]监听
    if(listen(tcp_socket,20) == -1)
    {
        perror("listen()");
        close(tcp_socket);
        return -3;
    }

    //[4]接受连接请求
    while(1)
    {
        new_socket = accept(tcp_socket,NULL,NULL);
        if(new_socket == -1)
        {
            perror("accept()");
            close(tcp_socket);
            return -4;
        }
        pid = fork();
        if(pid == -1)
        {
            perror("fork()");
            close(tcp_socket);
            return -5;
        }
        //[5]连接成功后，进行IO操作
        if(pid == 0)
        {
            write(new_socket,"Hello Amon",10);
            close(new_socket);
            close(tcp_socket);
            exit(0);
        }
        close(new_socket);
    }

    //[6]关闭套接字
    close(tcp_socket);

    return 0;
}

