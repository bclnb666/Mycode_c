#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h> // 注意：共享内存是 shm.h
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "portocol.h"

int main(void)
{
    // 1. 生成公共的 Key
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0) {
        perror("ftok()");
        return -1;
    }

    // 2. 获取或创建共享内存
    int shm_id = shmget(key, sizeof(struct shm_data), IPC_CREAT | 0600);
    if(shm_id == -1) {
        perror("shmget()");
        return -2;
    }

    // 3. 将共享内存映射到当前进程的地址空间
    struct shm_data *ptr = (struct shm_data *)shmat(shm_id, NULL, 0);
    if(ptr == (void *)-1) {
        perror("shmat()");
        return -3;
    }

    printf("接收端已启动，等待数据输入...\n");

    // 4. 循环监听数据
    while(1) {
        // 如果 flag 为 1，说明发送端已经把数据写好了
        if (ptr->flag == 1) {
            printf("收到消息: %s\n", ptr->buf);
            
            // 如果收到 quit，就退出循环
            if (strncmp(ptr->buf, "quit", 4) == 0) {
                printf("接收到退出指令，程序结束。\n");
                break;
            }

            // 【关键操作】：读取完数据后，将 flag 置回 0，告诉发送端“你可以继续写了”
            ptr->flag = 0; 
        }
        
        // 稍微睡一小会儿，防止死循环把 CPU 跑满（10000微秒 = 10毫秒）
        usleep(10000); 
    }

    // 5. 分离并销毁共享内存
    shmdt(ptr);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
