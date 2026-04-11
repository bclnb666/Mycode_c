/***********************************************************
* File Name:    outputer.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分57秒
***********************************************************/
#include "loghawk.h"
#include <sys/time.h>
#include <signal.h>
#include <sys/stat.h>

// 令牌桶配置: 初始令牌 0，每秒产生 10 个，最大积攒 50 个令牌
struct token_bucket tb = {0, 10, 50}; 

// 定时器信号(SIGALRM)处理函数：每秒增加令牌
void sigalrm_handler(int signo)
{
    if (tb.token < tb.burst)
    {
        tb.token += tb.cps;
        if (tb.token > tb.burst)
        {
            tb.token = tb.burst;
        }
    }
}

int outputer()
{
    printf("[Outputer] Module started successfully.\n");
    fflush(stdout);

    key_t msg_key = ftok(PATHNAME, PROJ_ID_MSG);
    int msg_id = msgget(msg_key, 0);

    //打开多个分类日志文件
    FILE *fp_info   = fopen("/tmp/loghawk_info.log", "a+");
    FILE *fp_warn   = fopen("/tmp/loghawk_warn.log", "a+");
    FILE *fp_error  = fopen("/tmp/loghawk_error.log", "a+");
    FILE *fp_kernel = fopen("/tmp/loghawk_kernel.log", "a+");

    if (!fp_info || !fp_warn || !fp_error || !fp_kernel) {
        printf("[Outputer] Failed to open log files.\n");
        exit(1);
    }

    // 设置 SIGALRM 信号响应机制
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigalrm_handler;
    sigaction(SIGALRM, &sa, NULL);

    //定时器，每秒触发一次 SIGALRM
    struct itimerval itv;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);

    struct msgbuf msg;

    while(1)
    {
        // 接收消息队列中的日志数据 (阻塞)
        if (msgrcv(msg_id, &msg, sizeof(msg.mtext), 0, 0) == -1)
        {
            if (errno == EINTR) continue; // 收到信号（如 SIGALRM）导致中断，继续即可
            sleep(1);
            continue;
        }

        // 令牌桶限流判断
        while (tb.token <= 0)
        {
            usleep(100000); // 无令牌时休眠 100 毫秒等待补充
        }

        tb.token--; // 消耗 1 个令牌

        //不同的消息类型写入不同的文件
        FILE *target_fp = fp_info; // 默认写入 info
        switch (msg.mtype) {
            case LOG_TYPE_INFO:   target_fp = fp_info; break;
            case LOG_TYPE_WARN:   target_fp = fp_warn; break;
            case LOG_TYPE_ERROR:  target_fp = fp_error; break;
            case LOG_TYPE_KERNEL: target_fp = fp_kernel; break;
        }

        // 写入到目标文件
        fprintf(target_fp, "%s\n", msg.mtext);
        fflush(target_fp);
    }

    fclose(fp_info);
    fclose(fp_warn);
    fclose(fp_error);
    fclose(fp_kernel);
    return 0;
}
