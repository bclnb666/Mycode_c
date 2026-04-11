#ifndef __PROTOCOL_H
#define __PROTOCOL_H

//[1]多播组的地址
#define MULTICAST_IP    "234.2.3.4"
//[2]在创建多播组的时候,需要本地的IP地址(写"0.0.0.0"的好处是将来换主机照样可以使用)
#define LOCAL_IP        "0.0.0.0"
//[3]通过ifconfig命令查看当前系统的网卡名字
#define NETCARD_NAME    "ens33"
//[4]接收端的端口
#define RECV_PORT       9527

//信息大小 (音频数据块大小，通常UDP建议在MTU以内)
#define MSGSIZE         1024

//[5]约定双方对话的格式
struct data_st
{
    char msg[MSGSIZE];
}__attribute__((packed));//单字节对齐

#endif
