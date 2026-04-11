#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "portocol.h"


int main(void)
{
    key_t key;
    int msg_id = 0;
    struct msg_st recv_buf;
    ssize_t count = 0;

    key = ftok(PATH,PROJ_ID);
    if(key == -1)
    {
        perror("ftok()");
        return -1;
    }
    
    msg_id = msgget(key,IPC_CREAT | IPC_EXCL | 0600);
    if(msg_id == -1)
    {
        if(errno == EEXIST)
        {
            msg_id = msgget(key,0);
        }
        else
        {
            perror("msgget()");
            return -2;
        }
    }
    while(1)
    {
        memset(&recv_buf,0,sizeof(recv_buf));
        count = msgrcv(msg_id,&recv_buf,STRSIZE,0,0);
        if(count == -1)
        {
            perror("msgrcv()");
            msgctl(msg_id,IPC_RMID,NULL);
            return -3;
        }
        puts(recv_buf.str);
    }

    return 0;
}











