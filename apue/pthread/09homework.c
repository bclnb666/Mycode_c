/***********************************************************
* File Name:    09homework.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月02日 星期四 20时02分58秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define NUM 5 

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// turn = 0 代表该 0号线程(打印a) 执行，以此类推
static int turn = 0; 

void alrm_handler(int sig)
{
    exit(0); 
}

void *thr_job(void *arg)
{
    int my_id = (int)(long)arg; 
   
    char my_char = 'a' + my_id; 

    while (1)
    {
        pthread_mutex_lock(&mutex);
        
        while (turn != my_id) 
        {            
            pthread_cond_wait(&cond, &mutex); 
        }

        putchar(my_char);       
        fflush(stdout); 
        turn = (turn + 1) % NUM; 
        pthread_cond_broadcast(&cond); 
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[NUM];
    int i, ret;

    signal(SIGALRM, alrm_handler);
    alarm(5); 

    for (i = 0; i < NUM; i++)
    {        
        ret = pthread_create(tid + i, NULL, thr_job, (void *)(long)i);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_create failed!\n");
            exit(1);
        }
    }
    
    for (i = 0; i < NUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
