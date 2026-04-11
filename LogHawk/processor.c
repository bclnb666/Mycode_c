/***********************************************************
* File Name:    processor.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分43秒
***********************************************************/
#include "loghawk.h"
#include <time.h>
#include <stdio.h>
#include <ctype.h>

int processor()
{
    // 获取 IPC 标识符
    key_t shm_key = ftok(PATHNAME, PROJ_ID_SHM);
    int shm_id = shmget(shm_key, sizeof(struct shm_buffer), 0);
    struct shm_buffer *shm = (struct shm_buffer *)shmat(shm_id, NULL, 0);

    key_t sem_key = ftok(PATHNAME, PROJ_ID_SEM);
    int sem_id = semget(sem_key, 3, 0);

    key_t msg_key = ftok(PATHNAME, PROJ_ID_MSG);
    int msg_id = msgget(msg_key, 0);

    printf("[Processor] PID %d started and waiting for data...\n", getpid());

    char raw_log[MSG_MAX];
    char (*blocks)[MSG_MAX] = (char (*)[MSG_MAX])shm->data;
    
    struct msgbuf msg;

    while(1)
    {
        // 1. 申请已有数据读入 (P 操作信号量 [2]，如果无数据将阻塞在这里)
        sem_p_ext(sem_id, 2);
        // 2. 申请互斥锁 (P 操作信号量 [0])
        sem_p_ext(sem_id, 0);

        // 从环形缓冲区读取
        strncpy(raw_log, blocks[shm->read_pos], MSG_MAX);
        shm->read_pos = (shm->read_pos + 1) % MAX_LOGS;

        // 3. 释放互斥锁 (V 操作信号量 [0])
        sem_v_ext(sem_id, 0);
        // 4. 释放空闲空间给 Collector (V 操作信号量 [1])
        sem_v_ext(sem_id, 1);

        // 为了匹配时不区分大小写，先弄个全小写的副本
        char lower_log[MSG_MAX];
        strncpy(lower_log, raw_log, MSG_MAX);
        for(int i = 0; lower_log[i]; i++) {
            lower_log[i] = tolower(lower_log[i]);
        }

        //关键字路由匹配
        if (strstr(lower_log, "error") || strstr(lower_log, "fail") || strstr(lower_log, "fatal")) {
            msg.mtype = LOG_TYPE_ERROR;
        } else if (strstr(lower_log, "warn")) {
            msg.mtype = LOG_TYPE_WARN;
        } else if (strstr(lower_log, "kernel") || strstr(lower_log, "usb") || strstr(lower_log, "eth")) {
            msg.mtype = LOG_TYPE_KERNEL;
        } else {
            msg.mtype = LOG_TYPE_INFO;
        }

        //处理/解析/格式化日志 ---
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

        
        int prefix_len = snprintf(msg.mtext, MSG_MAX, "[LogHawk Parsed %s] ", time_str);
        if (prefix_len >= 0 && prefix_len < MSG_MAX) {
            // 先确保 msg.mtext 是个以 '\0' 结尾的合法字符串
            // 然后算出还剩多少空间，使用 strncat 安全追加日志内容
            int remaining_space = MSG_MAX - prefix_len - 1;
            strncat(msg.mtext, raw_log, remaining_space);
        }

        // 发送到消息队列给 Outputer 模块
        if (msgsnd(msg_id, &msg, sizeof(msg.mtext), 0) == -1)
        {
            perror("[Processor] msgsnd()");
        }
    }

    shmdt(shm);
    return 0;
}
