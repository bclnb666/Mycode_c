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

// 守护进程化核心步骤
void daemonize()
{
    pid_t pid = fork();
    if(pid < 0) exit(1);
    if(pid > 0) exit(0); // 父进程退出

    setsid(); // 成为新会话首进程

    pid = fork();
    if(pid < 0) exit(1);
    if(pid > 0) exit(0); // 防止重新获得控制终端

    chdir("/"); // 更改目录为根目录
    umask(0);   // 重置文件权限掩码

    // 重定向标准输入输出流到 /dev/null 或专门的日志里
    freopen("/dev/null", "r", stdin);
    freopen("/tmp/loghawk_outputer_debug.log", "w", stdout);
    freopen("/tmp/loghawk_outputer_debug.log", "w", stderr);
}

int outputer()
{
    daemonize(); // 转为守护进程
    printf("[Outputer] Daemon started successfully.\n");
    fflush(stdout);

    key_t msg_key = ftok(PATHNAME, PROJ_ID_MSG);
    int msg_id = msgget(msg_key, 0);

    // 默认写出路径，普通用户运行无 /var/log 权限时会降级到 /tmp
    FILE *out_fp = fopen("/var/log/loghawk_final.log", "a+");
    if(out_fp == NULL) {
        out_fp = fopen("/tmp/loghawk_final.log", "a+");
    }

    // 设置 SIGALRM 信号响应机制
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigalrm_handler;
    sigaction(SIGALRM, &sa, NULL);

    // 设置高精度定时器，每秒触发一次 SIGALRM
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

        // 写入到最终文件/远端系统
        fprintf(out_fp, "%s\n", msg.mtext);
        fflush(out_fp);
    }

    fclose(out_fp);
    return 0;
}
