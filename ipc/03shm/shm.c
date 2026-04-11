/***********************************************************
* File Name:    shm.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月31日 星期二 16时38分22秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    
    int shm_id = shmget(IPC_PRIVATE,1024,IPC_CREAT | IPC_EXCL | 0600);
    int ret = 0;
    pid_t pid;
    void *ptr = NULL;

    if(shm_id == -1)
    {
        if(errno == EEXIST)
        {
            shm_id = shmget(IPC_PRIVATE,1024,0);
        }
        else
        {
            perror("shmget()");
            ret = -1;
            goto ERR_1;
        }
    }
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        ret = -2;
        goto ERR_2;
    }
    if(pid == 0)
    {
        ptr = shmat(shm_id,NULL,0);
        if(ptr == (void *)-1)
        {
            perror("shmat()");
            exit(1);
    
        }
        memcpy(ptr,"HELLO WORLD!",12);
        shmdt(ptr);
        exit(0);
    }

    wait(NULL);
    ptr = shmat(shm_id,NULL,0);
    if(ptr == (void *)-1)
    {
        perror("shmat()");
        ret = -3;
        goto ERR_2;
    }
    puts(ptr);
    shmdt(ptr);
    
ERR_2:
    shmctl(shm_id,IPC_RMID,NULL);
ERR_1:
    return ret;
}
