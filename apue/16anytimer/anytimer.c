#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "anytimer.h"

#define ANYTIMERMAX 1024//最大支持1024个闹钟

typedef struct
{
    int remain;//剩余秒数
    HANDLER callback;//闹钟响了之后执行的回调函数
    void *arg;//回调函数的参数
    int is_used;//标记当前闹钟是否被使用(1=使用中 0=未被使用)
}anytimer_st;

static anytimer_st timer_libs[ANYTIMERMAX];//闹钟库,存储所有闹钟实例
static int lib_inited = 0;//标记闹钟库是否初始化(0=未被初始化 1=已初始化)

static int get_anytimer_pos(void)
{
    int i = 0;//循环变量

    for(i = 0; i < ANYTIMERMAX; i++)//循环遍历
        if(timer_libs[i].is_used == 0)//判断当前位置的闹钟是否可用
            return i;//返回可用的标识
    return -1;//由于没有找到可用的位置,返回负数
}

static void alarm_handler(int none)//SIGALRM信号触发的行为,遍历所有闹钟,更剩余秒数
{
    int i = 0;//循环变量

    alarm(1);//设置1s的闹钟

    for(i = 0; i < ANYTIMERMAX; i++)//循环遍历所有闹钟
    {
        if(timer_libs[i].is_used == 1)//判断当前闹钟是否正在使用
        {
            timer_libs[i].remain--;//更新剩余秒数
            if(timer_libs[i].remain <= 0)//判断该闹钟是否到了定时时间
            {
                if(timer_libs[i].callback != NULL)////判断是否有回调函数
                {
                    timer_libs[i].callback(timer_libs[i].arg);//执行回调函数
                }
                timer_libs[i].is_used = 0;//执行完回调函数之后,把该闹钟标记为空闲状态
            }
        }
    }
}


static void lib_init(void)//初始化闹钟库
{
    int i = 0;//循环变量

    for(i = 0; i < ANYTIMERMAX; i++)//循环遍历初始化所有闹钟为空闲
    {
        timer_libs[i].remain = 0;
        timer_libs[i].callback = NULL;
        timer_libs[i].arg = NULL;
        timer_libs[i].is_used = 0;
    }
    signal(SIGALRM, alarm_handler);//设置SIGALRM信号的行为
    alarm(1);//设置1s的闹钟
    lib_inited = 1;//切换闹钟库初始化的状态(切换成已初始化状态)
}
 
int anytimer_init(int seconds, HANDLER handler, void *arg)
{
    int pos = 0;//pos存储当前闹钟库中可用最小的标识

    if(seconds <= 0 || handler == NULL)//判断形参是否不合法
        return -1;//由于形参不合法,结束函数,并且返回-1
    if(!lib_inited)//判断是否是第一次使用闹钟库
        lib_init();//由于是第一次使用闹钟库,进行初始化
    pos = get_anytimer_pos();//查找当前闹钟库中可用最小的标识
    if(pos < 0)//判断查找是否失败
        return -2;//由于查找当前闹钟库中可用最小的标识失败,结束函数,并且返回-2

    timer_libs[pos].remain = seconds;//存储客户指定的秒数
    timer_libs[pos].callback = handler;//存储客户指定的回调函数
    timer_libs[pos].arg = arg;//存储客户指定回调函数的参数
    timer_libs[pos].is_used = 1;//把当前闹钟的使用状态调整为正在使用

    return pos;//返回成功获取到的标识
}

int anytimer_destroy(int timer_id)
{
    if(timer_id < 0 || timer_id >= ANYTIMERMAX)//判断形参是否不合法
        return -1;//由于形参不合法,结束程序,并且返回-1
    if(timer_libs[timer_id].is_used == 0)//判断用户指定的闹钟是否未使用
        return -2;//由于用户指定的闹钟未使用,结束程序,并且返回-2
    //把用户指定的闹钟清空
    timer_libs[timer_id].remain = 0;
    timer_libs[timer_id].callback = NULL;
    timer_libs[timer_id].arg = NULL;
    timer_libs[timer_id].is_used = 0;

    return 0;
}
