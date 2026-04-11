#ifndef __LOGHAWK_
#define __LOGHAWK_

#define SHM_SIZE (1024 * 1024 * 4)  // 4MB
#define MSG_MAX 1024
#define MAX_LOGS (SHM_SIZE / MSG_MAX) // 环形缓冲区能装多少条日志 (约 4096 条)

// 【关键修复】将相对路径改为绝对路径，防止守护进程 chdir("/") 后导致 ftok 计算错位
#define PATHNAME "/tmp" 
#define PROJ_ID_SHM 0x11
#define PROJ_ID_SEM 0x22
#define PROJ_ID_MSG 0x33

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

struct shm_buffer
{
    int write_pos;    // 写指针
    int read_pos;     // 读指针
    char data[SHM_SIZE];
};

struct msgbuf
{
    long mtype;
    char mtext[MSG_MAX];
};

struct token_bucket
{
    int token;      // 当前令牌数
    int cps;        // 每秒生成令牌
    int burst;      // 最大容量
};

struct log_offset
{
    char filename[256];
    long offset;
    ino_t inode;
};

union semun
{
    int val;//信号量的初始值
};

// 功能 : 将信号量初始化为指定值 (使用 static inline 避免多重定义报错)
static inline void sem_init_ext(int semid, int sem_num, int val)
{
    union semun tmp;
    tmp.val = val;
    semctl(semid, sem_num, SETVAL, tmp);
}

// 兼容你原来的接口: 默认操作下标0
static inline void sem_init(int semid)
{
    sem_init_ext(semid, 0, 1);
}

// 功能 : 扩展版P操作 (支持操作指定下标的信号量)
static inline void sem_p_ext(int semid, int sem_num)
{
    struct sembuf buf;
    buf.sem_num = sem_num;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}

// 兼容你原来的接口: 默认操作下标0 (互斥锁)
static inline void sem_p(int semid)
{
    sem_p_ext(semid, 0);
}

// 功能 : 扩展版V操作 (支持操作指定下标的信号量)
static inline void sem_v_ext(int semid, int sem_num)
{
    struct sembuf buf;
    buf.sem_num = sem_num;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    semop(semid, &buf, 1);
}

// 兼容你原来的接口: 默认操作下标0 (互斥锁)
static inline void sem_v(int semid)
{
    sem_v_ext(semid, 0);
}

//-------------collector--------------
int collector();

//-------------processor---------------
int processor();

//-------------outputer----------------
int outputer();

#endif
