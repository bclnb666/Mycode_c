/***********************************************************
* File Name:    07prime_con.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 10时05分24秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MIN 100
#define MAX 300
#define NUM (MAX - MIN + 1)

union semun
{
    int val;
};

int *shm_ptr = NULL;
int semid;


void sem_init(int semid)
{
    union semun tmp;
    tmp.val = 1;
    semctl(semid,0,SETVAL,tmp);
}

void sem_p(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    semop(semid,&buf,1);
}
void sem_v(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    semop(semid,&buf,1);
}


int is_primer(int num)
{
    int i = 0;

    sleep(1);
    
    if(num <= 1)//判断num是否 <= 1
        return 0;
    if(num == 2 || num == 3)
        return 1;
    for(i = 2; i <= num / i; i++)
        if(num % i == 0)
            return 0;
    return 1;
}

void work(int n)
{
    int num = 0;
    while(1)
    {
        sem_p(semid);
        if(*shm_ptr > MAX)
        {
            sem_v(semid);
            break;
        }
        num = *shm_ptr;
        (*shm_ptr)++;
        sem_v(semid);
        if(is_primer(num))
            printf("[%d] %d Is a prime number!\n",n,num);
    }
}

int main(int argc, char *argv[]) {

    int i = 0;//循环变量
    int n = 0;//循环变量
    pid_t pid;//存储子进程的pid
    int shmid = 0;
    
    shmid = shmget(IPC_PRIVATE,4,IPC_CREAT | 0600);
    shm_ptr = shmat(shmid,NULL,0);
    *shm_ptr = MIN;

    semid = semget(IPC_PRIVATE,1,IPC_CREAT | 0600);
    sem_init(semid);

    for(n = 0; n < 4; n++)
    {
        pid = fork();//创建子进程
        if(pid < 0)//判断创建子进程是否失败
        {
            perror("fork()");//打印错误信息
            exit(1);//由于创建子进程失败,终止进程,并且返回状态1
        }
        if(pid == 0)
        {  
            work(n);
            exit(0);
        }
    }


    for(i = 0; i < 4; i++)//循环收尸
        wait(NULL);
    shmdt(shm_ptr);
    shmctl(shmid,IPC_RMID,NULL);
    semctl(semid,0,IPC_RMID);
    
    return 0;
}
