#ifndef __TBF_H
#define __TBF_H

/*
功能 : 初始化令牌桶
参数 : cps 客户指定的速率
        burst 客户指定的容量
返回值 : 成功返回令牌桶库的下标(>=0);失败返回负数(<0)
*/
int tbf_init(int cps, int burst);

/*
功能 : 取令牌
参数 : td 令牌桶库的描述符(令牌桶库的表示)
        n 客户要取走的令牌数
返回值 : 成功取走的令牌数
*/
int tbf_fetch_token(int td, int n);

/*
功能 : 销毁相应的令牌桶
参数 : td 令牌桶库的描述符(令牌桶库的表示)
返回值 : 成功返回0;失败返回负数
*/
int tbf_destroy(int td);

#endif

