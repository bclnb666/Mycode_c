/***********************************************************
* File Name:    outputer.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分57秒
***********************************************************/

#include "loghawk.h"
#include <time.h>

int outputer() {
    // 变成守护进程！脱离终端，在后台默默工作
    if(daemon(0, 0) < 0) {
        perror("daemon");
        exit(1);
    }

    int msg_id = msgget(ftok(PATHNAME, PROJ_ID_MSG), 0);
    
    // 打开最终的输出文件
    FILE *out_fp = fopen("output_result.log", "a");
    if(!out_fp) exit(1);

    // 初始化令牌桶 (每秒产出 2 个令牌，最大容量 5 个，防止瞬间写爆磁盘)
    struct token_bucket bucket = { .token = 5, .cps = 2, .burst = 5 };
    time_t last_time = time(NULL);

    struct msgbuf msg;
    while(1) {
        // 1. 从消息队列接收数据 (没数据就阻塞)
        msgrcv(msg_id, &msg, sizeof(msg.mtext), 1, 0);

        // 2. 令牌桶限流算法
        while (1) {
            time_t current_time = time(NULL);
            if (current_time > last_time) {
                // 每过去一秒，加 cps 个令牌，但不超过上限 burst
                bucket.token += bucket.cps * (current_time - last_time);
                if (bucket.token > bucket.burst) bucket.token = bucket.burst;
                last_time = current_time;
            }

            if (bucket.token > 0) {
                bucket.token--; // 消耗一个令牌
                break; // 拿到令牌，可以写盘了！
            }
            usleep(100000); // 没拿到令牌，稍微睡一会儿再重试
        }

        // 3. 写入最终目标文件
        fprintf(out_fp, "%s\n", msg.mtext);
        fflush(out_fp); // 立刻刷新到磁盘，方便查看
    }

    fclose(out_fp);
    return 0;
}
