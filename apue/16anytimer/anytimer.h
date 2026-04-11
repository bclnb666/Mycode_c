#ifndef __ANYTIMER_H
#define __ANYTIMER_H

typedef void(*HANDLER)(void *);//回调函数类型的定义

/*
功能 : 初始化闹钟库
参数 : seconds 要定时的秒数
        handler 要做的事情
        arg 要做的事情的参数
返回值 : 成功返回闹钟库的下标(>=0);失败返回负数(<0)
*/
int anytimer_init(int seconds, HANDLER handler, void *arg);

/*
功能 : 销毁指定标识的闹钟
参数 : timer_id 要销毁闹钟的标识
返回值 : 成功返回0;失败返回负数(<0)
*/
int anytimer_destroy(int timer_id);

#endif

