/***********************************************************
* File Name:    processor.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分43秒
***********************************************************/
#include "loghawk.h"
#include <time.h>
#include <stdio.h>

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
    msg.mtype = 1;

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

        // --- 核心职责: 处理/解析/格式化日志 ---
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

        // 简单打上时间戳并格式化 (使用 %.*s 明确限制 raw_log 的最大拷贝长度，预留 64 字节给前缀和时间戳，消除 truncation 警告)
        snprintf(msg.mtext, MSG_MAX, "[LogHawk Parsed %s] %.*s", time_str, (int)(MSG_MAX - 64), raw_log);
        msg.mtext[MSG_MAX - 1] = '\0'; // 确保字符串安全结束

        // 调试打印：让你在终端能看到进度
        printf("[Processor %d] Got a log! Sending to msg queue...\n", getpid());

        // 发送到消息队列给 Outputer 模块
        if (msgsnd(msg_id, &msg, sizeof(msg.mtext), 0) == -1)
        {
            perror("[Processor] msgsnd()");
        }
    }

    shmdt(shm);
    return 0;
}
