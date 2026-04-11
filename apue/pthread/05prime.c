/***********************************************************
* File Name:    01pthread.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月02日 星期四 10时22分56秒
***********************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MIN 100
#define MAX 300

#define NUM (MAX - MIN + 1)

static int is_prime(int num)
{
    int i = 0;//循环变量

    sleep(1);

    if(num <= 1)//判断num是否小于等于1(是否不是质数)
        return 0;
    if(num == 2 || num == 3)
        return 1;

    for(i = 2; i <= num / i; i++)
    {
        if(num % i == 0)
            return 0;
    }
    return 1;
}

void *thr_job(void *arg)
{
    int p = (int )arg;//强制类型转换

    if(is_prime(p))//判断是否是质数
        printf("%d Is A Prime Number!\n", p);
}

int main(void)
{
    int i = 0, j = 0;//循环变量
    pthread_t tid[NUM];//存储NUM个线程的标识
    int ret = 0;//存储返回值

    for(i = MIN, j = 0; i <= MAX; i++, j++)
    {
        ret = pthread_create(tid + j, NULL, thr_job, (void *)i);//创建线程
        if(ret != 0)//判断创建线程是否失败
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(ret));//打印错误信息
            exit(1);//由于创建线程失败,终止进程,并且设置状态为1
        }
    }

    //main()线程等待创建线程结束
    for(i = 0; i < NUM; i++)
        pthread_join(tid[i], NULL);

    return 0;
}

