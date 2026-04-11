/***********************************************************
* File Name:    processor.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 19时18分43秒
***********************************************************/
#include "loghawk.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int processor(int worker_id) {
    // 获取系统 IPC 资源
    int shm_id = shmget(ftok(PATHNAME, PROJ_ID_SHM), sizeof(struct shm_buffer), 0);
    int sem_id = semget(ftok(PATHNAME, PROJ_ID_SEM), 3, 0);
    int msg_id = msgget(ftok(PATHNAME, PROJ_ID_MSG), 0);
    
    if (shm_id < 0 || sem_id < 0 || msg_id < 0) {
        perror("[Processor] 无法获取 IPC 资源");
        return -1;
    }

    struct shm_buffer *shm = (struct shm_buffer *)shmat(shm_id, NULL, 0);
    
    printf("[Processor %d] 已启动，正在监听日志数据...\n", worker_id);

    while(1) {
        // 1. 申请数据: P(已有数据灯)
        // 如果没数据，直接挂起休眠，不消耗 CPU
        P(sem_id, 1);
        
        // --- 核心临界区开始 ---
        // 2. 申请互斥锁: P(互斥锁)
        // 确保同一时间只有 1 个进程能进入修改 read_pos
        P(sem_id, 2);
        
        // 3. 从共享内存取出数据并移动读指针
        char raw_data[MSG_MAX];
        int offset = shm->read_pos * MSG_MAX;
        strncpy(raw_data, shm->data + offset, MSG_MAX);
        shm->read_pos = (shm->read_pos + 1) % MAX_LOGS;
        
        // 4. 释放互斥锁: V(互斥锁)
        V(sem_id, 2);
        // --- 核心临界区结束 ---
        
        // 5. 释放车位: V(空闲空间灯)
        // 告诉 Collector 腾出了一个新位置
        V(sem_id, 0);

        // --- 开始处理数据 (脱离锁的限制，真正实现高并发处理) ---
        
        // 去除原始日志末尾自带的换行符，方便我们拼接新格式
        raw_data[strcspn(raw_data, "\r\n")] = 0; 
        
        struct msgbuf msg;
        msg.mtype = 1; // 消息队列的类型必须大于 0
        time_t t = time(NULL);
        
        // 格式化输出：打上工号和时间戳。使用 %.900s 限制字符串长度，消除截断警告
        snprintf(msg.mtext, MSG_MAX, "[Worker %d] [%ld] %.900s", worker_id, t, raw_data);
        
        // 将包装好的成品丢进消息队列，解耦完成！
        msgsnd(msg_id, &msg, sizeof(msg.mtext), 0);
        
        printf("   -> [Processor %d] 处理完毕，已压入消息队列.\n", worker_id);
    }

    shmdt(shm);
    return 0;
}
