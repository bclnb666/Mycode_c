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
#include <net/if.h>
#include <fcntl.h>
#include "protocol.h"

int main(int argc, char *argv[])
{
    int sd = 0;//存储创建成功的套接字描述符
    int fd = 0;//存储打开的音频文件描述符
    struct data_st buf;//存储读取和发送的数据
    struct sockaddr_in remote_addr;//存储对端的地址
    struct ip_mreqn imr;//存储组播选项的结构
    ssize_t read_bytes = 0;//存储成功读取到数据的字节数

    if(argc < 2)//判断命令行参数是否传递了音乐文件
    {
        fprintf(stderr, "Usage : %s <music_file>\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数不够,结束程序,并且返回-1
    }

    //[1]打开本地音乐文件
    fd = open(argv[1], O_RDONLY);//以只读方式打开音频文件
    if(fd == -1)//判断打开文件是否失败
    {
        perror("open()");//打印错误信息
        return -2;//由于打开文件失败,结束程序,并且返回-2
    }

    //[2]创建套接字
    sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字
    if(sd == -1)//判断创建报式套接字是否失败
    {
        perror("socket()");//打印错误信息
        close(fd);//关闭音频文件描述符
        return -3;//由于创建报式套接字失败,结束程序,并且返回-3
    }
    //bind忽略,套接字的地址由内核来绑定

    //[3]使能组播选项
    inet_aton(MULTICAST_IP, &imr.imr_multiaddr);//转换组播地址
    inet_aton(LOCAL_IP, &imr.imr_address);//转换本地地址
    imr.imr_ifindex = if_nametoindex(NETCARD_NAME);//转换网卡号
    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, \
                  &imr, sizeof(imr)) == -1)//判断使能多播组是否失败
    {
        perror("setsockopt()");//打印错误信息
        close(sd);//关闭套接字文件
        close(fd);//关闭音频文件描述符
        return -4;//由于使能多播组失败,结束程序,并且返回-4
    }

    //[4]准备多播组的目标地址
    remote_addr.sin_family = AF_INET;//指定IPv4协议
    inet_aton(MULTICAST_IP, &remote_addr.sin_addr);//填入多播组地址
    remote_addr.sin_port = htons(RECV_PORT);//填入端口号

    //[5]循环往多播组中发消息
    while(1)
    {
        memset(&buf, 0, sizeof(buf));//清空缓冲区
        read_bytes = read(fd, buf.msg, MSGSIZE);//从音频文件读数据
        
        if(read_bytes == -1)//判断读取是否失败
        {
            perror("read()");//打印错误信息
            break;
        }
        else if(read_bytes == 0)//判断是否读到了文件末尾
        {
            //要求组长的音乐循环播放
            lseek(fd, 0, SEEK_SET);//将文件偏移量重置到文件开头
            continue;
        }

        //往多播组中发送数据
        sendto(sd, &buf, read_bytes, 0, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
        
        //稍微慢一些，起到流量控制的作用，防止网络拥塞
        usleep(15000);//睡15毫秒左右(根据音频比特率可调整)
    }

    //[6]关闭描述符
    close(sd);//关闭套接字文件
    close(fd);//关闭音频文件描述符
    return 0;
}
