#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MIN 100
#define MAX 300

#define NUM 4//要创建的线程数

/*
job == 0    表示任务还没有发放
job >0      表示任务发放了,但是还未被取走
job == -1   表示所有任务发放完毕
*/
static int job;//临界区
/*
job变量是静态的全局变量,
所有的线程都回来访问\修改job,所以就是临界区
容易出现竞态,为了避免出现竞态,需要给临界区加互斥锁(互斥量)
*/
//[1]初始化互斥量(要么调用pthread_mutex_init(3)或者使用宏)
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//初始化互斥量

static int is_prime(int num)
{
    int i = 0;//循环变量

    //sleep(1);

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
/*
    循环抢任务
    [1]遇到job == -1    说明任务发放完毕->终止线程
    [2]遇到job == 0     说明还没有分配任务->等着抢任务
    [3]遇到job > 0      说明有任务->处理任务
*/
    int n = 0;//另存空间

    while(1)
    {
        pthread_mutex_lock(&mutex);//加锁
#if define(COND)
        while(job == 0)

        if(job == -1)//判断任务是否发放完毕
        {
            pthread_mutex_unlock(&mutex);//解锁
            pthread_exit(0);//终止线程
        }
        if(job == 0)//判断任务是否还没有发放
        {
            pthread_mutex_unlock(&mutex);//解锁
            continue;//跳过本次循环,继续下一次循环
        }
        n = job;//把临界资源拷贝一份儿
        job = 0;//修改临界区状态
        pthread_mutex_unlock(&mutex);//解锁
        if(is_prime(n))//判断是否是质数
            printf("%d Is A Prime Number!\n", n);
    }
}

int main(void)
{
    /*
    [1]初始化互斥量(要么调用pthread_mutex_init(3)或者使用宏)
    [2]创建所有的工作线程(pthread_create(3))
    [3]发放任务
    [4]告诉工作线程们,所有任务发放完毕
    [5]main()线程等待工作线程们结束
    [6]销毁互斥量
    */
    int i = 0;//循环变量
    pthread_t tid[NUM];//存储NUM个线程的标识
    int ret = 0;//存储返回值

    //[2]创建所有的工作线程(pthread_create(3))
    for(i = 0; i < NUM; i++)
    {
        ret = pthread_create(tid + i, NULL, thr_job, NULL);//创建线程
        if(ret != 0)//判断创建线程是否失败
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(ret));//打印错误信息
            exit(1);//由于创建线程失败,终止进程,并且设置状态为1
        }
    }

    //[3]发放任务
    for(i = MIN; i <= MAX; i++)
    {//我们需要等上一个任务被取走之后再分配新的任务
    //由于需要访问/修改job的值,所以需要先访问互斥量
        pthread_mutex_lock(&mutex);//加锁
        //如果互斥量已经被其他线程加锁了,会阻塞,直到这个线程拿到为止
        //能够执行到此为止,说明已经拿到互斥量了
        while(job > 0)//轮询等待job == 0(言外之意是轮询等待其他线程取走任务)
        {
            pthread_mutex_unlock(&mutex);//解锁
            pthread_mutex_lock(&mutex);//加锁
        }
        job = i;//main()线程发放新的任务
        pthread_mutex_unlock(&mutex);//解锁
    }

    //[4]告诉工作线程们,所有任务发放完毕(也就是 job = -1;)
    pthread_mutex_lock(&mutex);//加锁
    //如果互斥量已经被其他线程加锁了,会阻塞,直到这个线程拿到为止
    //能够执行到此为止,说明已经拿到互斥量了
    while(job > 0)//轮询等待job == 0(言外之意是轮询等待其他线程取走任务)
    {
        pthread_mutex_unlock(&mutex);//解锁
        pthread_mutex_lock(&mutex);//加锁
    }
    job = -1;//main()线程改变临界区状态为发放完毕
    pthread_mutex_unlock(&mutex);//解锁

    //[5]main()线程等待工作线程们结束
    for(i = 0; i < NUM; i++)
        pthread_join(tid[i], NULL);

    //[6]销毁互斥量
    pthread_mutex_destroy(&mutex);//销毁互斥量

    return 0;
}

