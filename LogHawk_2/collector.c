/***********************************************************
* File Name:    collector.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分27秒
***********************************************************/

#include "loghawk.h"

int collector()
{
    // 重新获取已创建的 IPC 标识符
    key_t shm_key = ftok(PATHNAME, PROJ_ID_SHM);
    int shm_id = shmget(shm_key, sizeof(struct shm_buffer), 0);
    struct shm_buffer *shm = (struct shm_buffer *)shmat(shm_id, NULL, 0);

    key_t sem_key = ftok(PATHNAME, PROJ_ID_SEM);
    int sem_id = semget(sem_key, 3, 0);

    // 尝试打开系统日志 (可能需要 sudo 权限)
    FILE *fp = fopen("/var/log/syslog", "r");
    if(fp == NULL)
    {
        perror("[Collector] fopen()");
        return -1;
    }

    // 简化的断点续传：直接定位到文件末尾，只采集系统新增的日志
    fseek(fp, 0, SEEK_END);
    printf("[Collector] Started monitoring /var/log/syslog\n");

    char buffer[MSG_MAX];
    char (*blocks)[MSG_MAX] = (char (*)[MSG_MAX])shm->data; // 将 4MB 数据区视为二维数组

    while(1)
    {
        if(fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            // 去除换行符
            buffer[strcspn(buffer, "\n")] = 0;
            if(strlen(buffer) == 0) continue;

            // 1. 申请空闲写空间 (P 操作信号量 [1])
            sem_p_ext(sem_id, 1);
            // 2. 申请互斥锁 (P 操作信号量 [0])
            sem_p_ext(sem_id, 0);

            // 将日志写入环形缓冲区的指定块
            strncpy(blocks[shm->write_pos], buffer, MSG_MAX - 1);
            blocks[shm->write_pos][MSG_MAX - 1] = '\0';
            shm->write_pos = (shm->write_pos + 1) % MAX_LOGS;

            // 3. 释放互斥锁 (V 操作信号量 [0])
            sem_v_ext(sem_id, 0);
            // 4. 增加可读数据计数，通知 Processor (V 操作信号量 [2])
            sem_v_ext(sem_id, 2);
        }
        else
        {
            // 文件暂时无新内容，清除 EOF 标志并稍作休眠 (轮询机制)
            clearerr(fp);
            usleep(100000); // 休眠 100 毫秒
        }
    }

    shmdt(shm);
    fclose(fp);
    return 0;
}

