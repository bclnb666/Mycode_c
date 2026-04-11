#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define SERVER_PORT     9527
#define MSGSIZE         128

struct data_st
{
    int8_t id;
    char msg[MSGSIZE];
}__attribute((packed));

#endif













