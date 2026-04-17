/* ipc_modules.c - System V IPC 共享内存、信号量数组与消息队列模块 */
#include "iot_gateway.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define IPC_KEY_PATH "/var/run/iot_gateway.pid" /* 使用稳定的锁文件路径计算基准令牌 */
#define IPC_KEY_PROJ_ID 'A'
#define SHM_CACHE_SIZE 8192

/* System V 信号量标准化配置结构，部分POSIX不自动提供此定义 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

static int sysv_shm_id = -1;
static int sysv_sem_id = -1;
static int sysv_msg_id = -1;
static void *shm_mapped_addr = (void *)-1;

int ipc_system_init(void) {
    /* 通过统一的文件路径和标量生成 System V 标准系统通信键值 */
    key_t ipc_key = ftok(IPC_KEY_PATH, IPC_KEY_PROJ_ID);
    if (ipc_key == -1) return -1;

    /* 1. 构建共享内存池：向内核申请一片固定尺寸的连续物理页映射 */
    sysv_shm_id = shmget(ipc_key, SHM_CACHE_SIZE, IPC_CREAT | 0666);
    if (sysv_shm_id == -1) return -1;

    /* 挂载共享物理内存至当前进程虚拟地址空间 */
    shm_mapped_addr = shmat(sysv_shm_id, NULL, 0);
    if (shm_mapped_addr == (void *)-1) return -1;

    /* 2. 构建信号量控制阵列：只包含 1 个原子计数器，用于充当二元 Mutex 锁 */
    sysv_sem_id = semget(ipc_key, 1, IPC_CREAT | 0666);
    if (sysv_sem_id == -1) return -1;

    union semun arg;
    arg.val = 1; /* 初始值释放态设定为 1 */
    if (semctl(sysv_sem_id, 0, SETVAL, arg) == -1) return -1;

    /* 3. 构建进程间结构体消息队列，用于业务模块解耦 */
    sysv_msg_id = msgget(ipc_key, IPC_CREAT | 0666);
    if (sysv_msg_id == -1) return -1;

    return 0;
}

/* 执行信号量的阻塞性 P 操作 (Lock 锁定物理内存区) */
static void ipc_shm_lock(void) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;      /* 占用资源减法 */
    /* 注入 SEM_UNDO：若当前进程崩溃，系统自动归还此计数器避免大面积死锁 */
    sb.sem_flg = SEM_UNDO; 
    semop(sysv_sem_id, &sb, 1);
}

/* 执行信号量的解除 V 操作 (Unlock 释放物理内存区) */
static void ipc_shm_unlock(void) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;       /* 释放资源加法 */
    sb.sem_flg = SEM_UNDO;
    semop(sysv_sem_id, &sb, 1);
}

/* 核心调用：在多进程并发环境下安全更迭传感器状态缓存 */
void publish_sensor_data_to_shm(const char *raw_data) {
    if (shm_mapped_addr == (void *)-1 ||!raw_data) return;
    
    ipc_shm_lock();
    /* 原子区内进行无干扰的数据拷贝 */
    strncpy((char *)shm_mapped_addr, raw_data, SHM_CACHE_SIZE - 1);
    ((char *)shm_mapped_addr) = '\0';
    ipc_shm_unlock();
}

void ipc_system_cleanup(void) {
    /* 脱离并抹除由内核分配的内存页与计数器标号 */
    if (shm_mapped_addr!= (void *)-1) shmdt(shm_mapped_addr);
    if (sysv_shm_id!= -1) shmctl(sysv_shm_id, IPC_RMID, NULL);
    if (sysv_sem_id!= -1) semctl(sysv_sem_id, 0, IPC_RMID, NULL);
    if (sysv_msg_id!= -1) msgctl(sysv_msg_id, IPC_RMID, NULL);
}
