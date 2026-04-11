/***********************************************************
* File Name:    loghawk.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 18时36分13秒
***********************************************************/

#include "loghawk.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("[Main] 正在初始化系统 IPC 资源...\n");

    // 1. 生成统一的 IPC Keys
    key_t shm_key = ftok(PATHNAME, PROJ_ID_SHM);
    key_t sem_key = ftok(PATHNAME, PROJ_ID_SEM);
    key_t msg_key = ftok(PATHNAME, PROJ_ID_MSG);

    // 2. 创建 共享内存、信号量集 (3个灯)、消息队列
    int shm_id = shmget(shm_key, sizeof(struct shm_buffer), IPC_CREAT | 0666);
    int sem_id = semget(sem_key, 3, IPC_CREAT | 0666);
    int msg_id = msgget(msg_key, IPC_CREAT | 0666);

    if (shm_id < 0 || sem_id < 0 || msg_id < 0) {
        perror("[Main] IPC 资源创建失败");
        return -1;
    }

    // 3. 格式化共享内存
    struct shm_buffer *shm = (struct shm_buffer *)shmat(shm_id, NULL, 0);
    shm->write_pos = 0; // 写指针归零
    shm->read_pos = 0;  // 读指针归零
    shmdt(shm);

    // 4. 初始化 3个信号量 (红绿灯)
    union semun arg;
    arg.val = MAX_LOGS;
    semctl(sem_id, 0, SETVAL, arg); // 灯0: 空闲空间 (初始=满)

    arg.val = 0;
    semctl(sem_id, 1, SETVAL, arg); // 灯1: 已有数据 (初始=0)

    arg.val = 1;
    semctl(sem_id, 2, SETVAL, arg); // 灯2: 互斥锁 (防多个Processor打架，初始=1)

    printf("[Main] IPC 资源就绪，准备拉起进程池...\n");

    // ================= 启动进程流水线 =================

    // 步骤 A: 拉起 1 个 Outputer 进程 (消费者末端)
    // 注意：这里假设你已经有了 outputer.c，如果尚未编写，可暂时将这段注释掉
    if (fork() == 0) {
        outputer();
        exit(0);
    }

    // 步骤 B: 拉起 3 个 Processor 进程 (并发处理池)
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            processor(i + 1); // 传入 1, 2, 3 作为工号
            exit(0);
        }
    }

    // 步骤 C: 拉起 1 个 Collector 进程 (生产者源头)
    if (fork() == 0) {
        collector();
        exit(0);
    }

    // ==================================================

    // 父进程挂起，充当系统守护者，等待所有子进程结束并替它们收尸
    printf("[Main] 流水线运行中... (按 Ctrl+C 退出)\n");
    while(wait(NULL) > 0);

    // 销毁 IPC 资源 (清理战场)
    printf("[Main] 正在清理 IPC 资源并安全退出系统...\n");
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}
