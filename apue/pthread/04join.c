/***********************************************************
* File Name:    01pthread.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月02日 星期四 10时22分56秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *thr_job(void *arg)
{
    for(int i = 0;i < 10;i++)
    {
        if(i == 5)
            pthread_exit((void *)0);

        write(1,"62",2);
        sleep(1);
    }
}
int main(int argc, char *argv[]) {
    
    pthread_t tid;
    int ret = 0;

    ret = pthread_create(&tid,NULL,thr_job,NULL);
    if(ret != 0)
    {
        fprintf(stderr,"pthread_create() is failed! errnum:%s\n",strerror(ret));
        exit(1);
    }
    while(1)
    {
        write(1,"$",1);
        sleep(1);
    }
    
    return 0;
}
