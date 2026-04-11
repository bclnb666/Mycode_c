/***********************************************************
* File Name:    loghawk.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 18时36分13秒
***********************************************************/
#include <stdio.h>
#include "loghawk.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

// 守护进程
void daemonize()
{
    pid_t pid = fork();
    if(pid < 0) exit(1);
    if(pid > 0) exit(0); // 父进程退出，立即释放终端

    setsid(); // 成为新会话首进程

    pid = fork();
    if(pid < 0) exit(1);
    if(pid > 0) exit(0); // 防止重新获得控制终端

    chdir("/"); // 更改目录为根目录
    umask(0);   // 重置文件权限掩码

    // 重定向标准输入输出流：整个系统所有的 printf 和 perror 都会被写进这个文件
    freopen("/dev/null", "r", stdin);
    freopen("/tmp/loghawk_system.log", "a+", stdout);
    freopen("/tmp/loghawk_system.log", "a+", stderr);
}

int main(int argc, char *argv[]) {

    // 0. 将整个主进程及未来所有的子进程全部转为后台守护进程
    daemonize();
    
    // 刷新一下缓冲区，确保日志写入
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("[Main] System daemonized. Parent PID: %d\n", getpid());

    // 1. 在主进程中提前创建好所有 IPC 资源，避免子进程启动时竞争和异常
    
    // 创建共享内存
    key_t shm_key = ftok(PATHNAME, PROJ_ID_SHM);
    int shm_id = shmget(shm_key, sizeof(struct shm_buffer), IPC_CREAT | 0666);
    void *ptr = shmat(shm_id, NULL, 0);
    struct shm_buffer *shm = (struct shm_buffer *)ptr;
    shm->write_pos = 0;
    shm->read_pos = 0;
    shmdt(ptr);

    // 创建信号量: [0]互斥锁, [1]可写空间, [2]可读数据
    key_t sem_key = ftok(PATHNAME, PROJ_ID_SEM);
    int sem_id = semget(sem_key, 3, IPC_CREAT | 0666);
    sem_init_ext(sem_id, 0, 1);          // 互斥锁，初值 1
    sem_init_ext(sem_id, 1, MAX_LOGS);   // 空闲可写空间，初值 4096
    sem_init_ext(sem_id, 2, 0);          // 已写入可读数据，初值 0

    // 创建消息队列
    key_t msg_key = ftok(PATHNAME, PROJ_ID_MSG);
    msgget(msg_key, IPC_CREAT | 0666);

    printf("[Main] System initialized. Starting modules...\n");

    // 2. 启动 Outputer
    pid_t pid_out = fork();
    if(pid_out == 0) {
        outputer();
        exit(0);
    }

    // 3. 启动 Processor
    int processor_cnt = 2;
    for(int i = 0; i < processor_cnt; ++i) {
        pid_t pid_proc = fork();
        if(pid_proc == 0) {
            processor();
            exit(0);
        }
    }

    // 4. 启动 Collector 模块 (采集数据)
    pid_t pid_col = fork();
    if(pid_col == 0) {
        collector();
        exit(0);
    }

    // 主进程阻塞等待子进程
    for(int i = 0; i < processor_cnt + 2; ++i) {
        wait(NULL);
    }

    return 0;
}
