#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "portocol.h"


int main(int argc,char *argv[])
{
    key_t key;
    int msg_id = 0;
    struct msg_st send_buf;
    ssize_t count = 0;

    if(argc < 3)
    {
        fprintf(stderr,"Usage : %s + id +msg\n",argv[0]);
        return -1;
    }

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
            return -3;
        }
    }
   
    memset(&send_buf,0,sizeof(send_buf));
    send_buf.mtype = atoi(argv[1]);
    strncpy(send_buf.str,argv[2],STRSIZE);

    count = msgsnd(msg_id,&send_buf,strlen(send_buf.str),0);
    if(count == -1)
    {
        perror("msgsnd()");
        msgctl(msg_id,IPC_RMID,NULL);
        return -4;
    }

    return 0;
}











