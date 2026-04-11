#ifndef __LOGHAWK_
#define __LOGHAWK_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PATHNAME "."
#define PROJ_ID_SHM 0x11
#define PROJ_ID_SEM 0x22
#define PROJ_ID_MSG 0x33

#define SHM_SIZE (1024 * 1024 * 4)  // 4MB
#define MSG_MAX 1024                // 单条日志最大长度
#define MAX_LOGS (SHM_SIZE / MSG_MAX) // 环形缓冲区能装多少条日志 (约 4096 条)

// === 1. 共享内存结构 (环形队列) ===
struct shm_buffer
{
    int write_pos;    // 写指针 (0 ~ MAX_LOGS-1)
    int read_pos;     // 读指针 (0 ~ MAX_LOGS-1)
    char data[SHM_SIZE]; // 真正存放数据的 4MB 空间
};

// === 2. 消息队列结构 ===
struct msgbuf
{
    long mtype;           // 消息类型，必须 > 0
    char mtext[MSG_MAX];  // 经过处理后的日志正文
};

// === 3. 令牌桶结构 ===
struct token_bucket
{
    int token;      // 当前令牌数
    int cps;        // 每秒生成令牌 (Count Per Second)
    int burst;      // 最大容量 (桶深)
};

// === 4. 信号量联合体 (用于初始化) ===
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// === 5. 模块函数声明 ===
int collector();
int processor(int worker_id);
int outputer();

// === P/V 操作内联工具函数 (方便所有文件直接调用) ===
static inline void P(int sem_id, int sem_num) {
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = -1; // 申请资源
    op.sem_flg = 0;
    semop(sem_id, &op, 1);
}

static inline void V(int sem_id, int sem_num) {
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = 1;  // 释放资源
    op.sem_flg = 0;
    semop(sem_id, &op, 1);
}


#endif
