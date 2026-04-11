#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "portocol.h"

int main(int argc, char *argv[])
{
    // 判断参数，argv[0]是程序名，argv[1]是你要发送的字符串
    if(argc < 2) {
        fprintf(stderr,"用法: %s <你要发送的消息>\n", argv[0]);
        return -1;
    }
   
    // 1. 使用与 recv.c 相同的路径和 ID，生成相同的 Key
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0) {
        perror("ftok()");
        return -1;
    }

    // 2. 获取共享内存 (因为 recv 通常先启动，所以这里直接获取即可)
    int shm_id = shmget(key, sizeof(struct shm_data), IPC_CREAT | 0600);
    if(shm_id == -1) {
        perror("shmget()");
        return -2;
    }
   
    // 3. 映射到当前进程地址空间
    struct shm_data *ptr = (struct shm_data *)shmat(shm_id, NULL, 0);
    if(ptr == (void *)-1) {
        perror("shmat()");
        return -3;         
    }

    // 4. 检查标志位，如果 flag == 1，说明上一条消息接收端还没来得及读，必须等待
    while (ptr->flag == 1) {
        printf("对方尚未读取上一条数据，等待中...\n");
        sleep(1);
    }

    // 5. 写入数据
    // 使用 strncpy 安全拷贝，并将参数从 argv[2] 改为正确的 argv[1]
    strncpy(ptr->buf, argv[1], BUFSIZE - 1);
    ptr->buf[BUFSIZE - 1] = '\0'; // 确保字符串一定有结束符
    
    // 【关键操作】：数据写完后，把标志位改为 1，通知接收端可以读了
    ptr->flag = 1; 
    
    printf("发送成功: %s\n", ptr->buf);

    // 6. 分离共享内存
    shmdt(ptr);

    return 0;
}
