/***********************************************************
* File Name:    collector.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分27秒
***********************************************************/
#include "loghawk.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int collector() {
    // 获取已经创建好的 IPC 资源 ID
    int shm_id = shmget(ftok(PATHNAME, PROJ_ID_SHM), sizeof(struct shm_buffer), 0);
    int sem_id = semget(ftok(PATHNAME, PROJ_ID_SEM), 3, 0);

    if (shm_id < 0 || sem_id < 0) {
        perror("[Collector] 无法获取 IPC 资源");
        return -1;
    }

    struct shm_buffer *shm = (struct shm_buffer *)shmat(shm_id, NULL, 0);

    // 模拟打开目标日志文件
    FILE *fp = fopen("test_log.txt", "r");
    if(!fp) {
        perror("[Collector] 找不到 test_log.txt (请新建该文件并写入几行测试文本)");
        return -1;
    }

    char line_buf[MSG_MAX];

    // 逐行读取日志
    while(fgets(line_buf, MSG_MAX, fp) != NULL) {

        // 1. 申请车位: P(空闲空间灯)
        // 如果环形队列满了，采集器会自动在这里阻塞等待
        P(sem_id, 0);

        // 2. 写入数据到共享内存的准确位置
        int offset = shm->write_pos * MSG_MAX; // 计算当前指针对应的字节偏移
        strncpy(shm->data + offset, line_buf, MSG_MAX - 1);

        // 3. 移动写指针 (取余数实现环形循环)
        shm->write_pos = (shm->write_pos + 1) % MAX_LOGS;

        // 4. 释放数据: V(已有数据灯)
        // 唤醒因为没数据而正在休眠的 Processor
        V(sem_id, 1);

        printf("[Collector] 抓取到新数据并入库: %s", line_buf);

        // 模拟真实场景下的数据产生间隔 (200毫秒)
        usleep(200000);
    }

    fclose(fp);
    shmdt(shm);
    printf("[Collector] 日志文件读取完毕，采集器安全退出。\n");
    return 0;
}
