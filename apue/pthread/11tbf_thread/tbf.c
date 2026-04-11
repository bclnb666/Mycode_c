#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "tbf.h"

#define TBFMAX 1024

typedef struct tbf_st
{   
    int token;//表示当前令牌桶的令牌数
    int cps;//表示当前令牌桶的速率
    int burst;//当前令牌桶的容量
#if defined(PTHREAD)
    pthread_mutex_t mutex;//表示当前令牌桶的互斥量
    pthread_cond_t cond;//表示当前令牌桶的条件变量
#endif
}tbf_t;//定义了令牌桶的数据结构

static tbf_t *tbf_libs[TBFMAX];//令牌桶库的定义
static int initd;//作为是否启动了令牌桶库的标记
//(0表示没有工作,1表示开始工作)
#if defined(PTHREAD)
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;//初始化令牌桶库的互斥量
static pthread_t jobtid;//定义全局的线程标识符
#endif

static int get_tbf_pos(void)//循环遍历令牌桶库中当前可用最小的令牌桶标记
{   
    int i = 0;//循环变量
    
    for(i = 0; i < TBFMAX; i++)//循环遍历
        if(tbf_libs[i] == NULL)//判断当前位置是否可用
            return i;//返回当前可用位置的下标
    return -1;//由于没有找到,返回-1
}

#if defined(PTHREAD)
static void *thr_fun(void *arg)//线程要做的工作
{   
    int i = 0;//循环变量
    
    while(1)
    {
        pthread_mutex_lock(&mut_job);//抢令牌桶库的锁
        for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
        {
            if(tbf_libs[i] != NULL)//判断该位置的令牌桶是否在用
            {
                pthread_mutex_lock(&tbf_libs[i]->mutex);//抢指定令牌桶中的锁
                tbf_libs[i]->token += tbf_libs[i]->cps;//积攒令牌
                if(tbf_libs[i]->token > tbf_libs[i]->burst)//判断指定令牌桶中的令牌是否超过了上限
                    tbf_libs[i]->token = tbf_libs[i]->burst;//让指定令牌桶中的令牌等于上限值
                pthread_mutex_unlock(&tbf_libs[i]->mutex);//解除指定令牌桶中的锁
                pthread_cond_broadcast(&tbf_libs[i]->cond);//让指定令牌桶发送条件变量变化的通知
            }
        }
        pthread_mutex_unlock(&mut_job);//解除令牌桶库的锁
        sleep(1);//睡1s(预留取消位置)
    }
    pthread_exit(NULL);//终止线程
}

static void module_unload(void)//卸载模块
{
    int i = 0;//循环变量

    pthread_cancel(jobtid);//给jobtid线程发送取消请求
    pthread_join(jobtid, NULL);//等待jobtid线程结束

    for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
    {
        if(tbf_libs[i] != NULL)//判断当前令牌桶是否可用
            tbf_destroy(i);//释放该令牌桶
    }
}

static void module_load(void)//加载模块
{
    int ret = 0;//用来存储返回值

    ret = pthread_create(&jobtid, NULL, thr_fun, NULL);//创建线程
    if(ret != 0)//判断创建线程是否失败
    {
        fprintf(stderr, "pthread_create():%s\n", strerror(ret));//打印错误信息
        exit(1);//由于创建线程失败,终止进程,并且设置状态为1
    }

    atexit(module_unload);//设置进程终止处理函数
}
#else
static void alarm_handler(int none)//SIGALRM信号的行为
{
    int i = 0;//循环变量

    alarm(1);//设置1s的闹钟
    for(i = 0; i < TBFMAX; i++)//循环遍历
    {
        if(tbf_libs[i] != NULL)//判断当前令牌桶是否正在使用
        {
            tbf_libs[i]->token += tbf_libs[i]->cps;//给当前令牌桶积攒该令牌桶指定的令牌
            if(tbf_libs[i]->token > tbf_libs[i]->burst)
            //判断当前令牌桶中令牌数是否超过该令牌桶指定的上限
                tbf_libs[i]->token = tbf_libs[i]->burst;
                //把当前令牌桶中令牌数设置为该令牌桶指定的上限
        }
    }
}

static void module_load(void)//启动模块
{
    signal(SIGALRM, alarm_handler);//给SIGALRM信号设置行为
    alarm(1);//设置1s的闹钟
}
#endif

int tbf_init(int cps, int burst)
{
    int pos = 0;//pos变量存储查询到的当前可用最小的令牌桶库的标记

    if(cps <= 0 || burst <= 0)//判断形参是否没有意义
        return -1;//由于形参没有意义,结束函数,并且返回-1
    if(!initd)//判断令牌桶库是否没有工作
    {
        module_load();//启动模块
        initd = 1;//切换状态为工作状态
    }
    pos = get_tbf_pos();//获取当前令牌桶库中可用的最小的令牌桶标记
    if(pos < 0)//判断是否没有找到位置
        return -2;//由于没有找到位置,结束函数,并且返回-2

    tbf_libs[pos] = malloc(sizeof(tbf_t));//为令牌桶开辟空间
    if(tbf_libs[pos] == NULL)//判断为令牌桶开辟空间是否失败
        return -3;//由于为令牌桶开辟空间失败,结束函数,并且返回-3
    tbf_libs[pos]->cps = cps;//存储客户指定的速率
    tbf_libs[pos]->burst = burst;//存储客户指定的上限
    tbf_libs[pos]->token = 0;//初始化令牌桶中的令牌数为0
#if defined(PTHREAD)
    pthread_mutex_init(&tbf_libs[pos]->mutex, NULL);//初始化令牌桶中的互斥量
    pthread_cond_init(&tbf_libs[pos]->cond, NULL);//初始化令牌桶中的条件变量
#endif
    return pos;//返回当前初始化成功的令牌桶下标返回
}

#if defined(PTHREAD)
static int getmin(int m, int n)
{
    return m > n ? n : m;
}
#endif

int tbf_fetch_token(int td, int n)
{
    int fetch_token = 0;//存储取走的令牌数

    if(td < 0 || td >= TBFMAX || n <= 0)//判断参数是否没有意义
        return -1;//由于参数没有意义,结束函数,并且返回-1
    if(tbf_libs[td] == NULL)//判断td标记的令牌桶是否不存在
        return -2;//由于td标记的令牌桶不存在,结束函数,并且返回-2
#if defined(PTHREAD)
    pthread_mutex_lock(&tbf_libs[td]->mutex);//抢指定令牌桶的锁
    while(tbf_libs[td]->token <= 0)//等待td令牌桶内积攒令牌
        pthread_cond_wait(&tbf_libs[td]->cond, &tbf_libs[td]->mutex);//等待指定令牌桶的条件变量变化
    fetch_token = getmin(tbf_libs[td]->token, n);//判断客户需要和令牌桶内的令牌数哪个更小
    tbf_libs[td]->token -= fetch_token;//更新td令牌桶中的令牌数
    pthread_mutex_unlock(&tbf_libs[td]->mutex);//解除指定令牌桶的锁
#else
    while(tbf_libs[td]->token <= 0)//等待td令牌桶内积攒令牌
        pause();//等通知
    if(tbf_libs[td]->token >= n)//判断td令牌桶内的令牌是否充足
        fetch_token = n;//取走客户指定的令牌个数
    else//td令牌桶内的令牌不充足
        fetch_token = tbf_libs[td]->token;//有多少,取多少
    tbf_libs[td]->token -= fetch_token;//更新td令牌桶内的令牌数
#endif

    return fetch_token;//返回成功取走的令牌数
}

int tbf_destroy(int td)
{
    if(td < 0 || td >= TBFMAX)//判断参数是否没有意义
        return -1;//由于参数没有意义,结束函数,并且返回-1
    if(tbf_libs[td] == NULL)//判断td标记的令牌桶是否不存在
        return -2;//由于td标记的令牌桶不存在,结束函数,并且返回-2
#if defined(PTHREAD)
    pthread_mutex_destroy(&tbf_libs[td]->mutex);//释放互斥量
    pthread_cond_destroy(&tbf_libs[td]->cond);//释放条件变量
#endif
    free(tbf_libs[td]);//释放td令牌桶
    tbf_libs[td] = NULL;//避免出现野指针

    return 0;//释放成功,返回0
}

#if defined(PTHREAD)
int tbf_return_token(int td, int ntoken)
{
    if(td < 0 || td >= TBFMAX || ntoken <= 0)//判断形式参数是否不合理
        return -1;//由于形式参数不合理,结束函数,并且返回-1
    if(tbf_libs[td] == NULL)//判断td标识的令牌桶是否不存在
        return -2;//由于td标识的令牌桶不存在,结束函数,并且返回-2
    pthread_mutex_lock(&tbf_libs[td]->mutex);//抢指定令牌桶的锁
    tbf_libs[td]->token += ntoken;//返还令牌
    if(tbf_libs[td]->token > tbf_libs[td]->burst)//判断返还令牌后令牌数是否超过上限
        tbf_libs[td]->token = tbf_libs[td]->burst;//将令牌桶中的令牌数设定为上限值
    pthread_mutex_unlock(&tbf_libs[td]->mutex);//解除指定令牌桶的锁
    pthread_cond_broadcast(&tbf_libs[td]->cond);//给指定令牌桶中发送条件变量变化的通知

    return 0;
}

void tbf_destroy_all(void)
{
    int i = 0;//循环变量

    pthread_mutex_lock(&mut_job);//抢令牌桶库的锁

    for(i = 0; i < TBFMAX; i++)//循环销毁令牌桶
    {
        if(tbf_libs[i] != NULL)
            tbf_destroy(i);
    }

    pthread_mutex_unlock(&mut_job);//解除令牌桶库的锁
}

#endif

