/***********************************************************
* File Name:    09mycat_sem.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 14时52分46秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

#define CHILD_COUNT 4

int main(int argc, char *argv[]) {
    // 1. 检查参数
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    struct stat st;
    
    // 2. 获取文件状态和大小
    if (stat(filename, &st) < 0) {
        perror("获取文件状态失败");
        return 1;
    }

    off_t total_size = st.st_size;
    off_t chunk_size = total_size / CHILD_COUNT;

    // 3. 创建共享内存用于存放信号量
    // 使用 mmap 映射一块匿名共享内存，供父子进程共享
    sem_t *sems = mmap(NULL, CHILD_COUNT * sizeof(sem_t), PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sems == MAP_FAILED) {
        perror("共享内存映射(mmap)失败");
        return 1;
    }

    // 4. 初始化信号量
    // 第 0 个子进程的信号量初始值为 1 (可以直接运行)
    // 第 1, 2, 3 个子进程的信号量初始值为 0 (需要等待被唤醒)
    sem_init(&sems[0], 1, 1); 
    for (int i = 1; i < CHILD_COUNT; i++) {
        sem_init(&sems[i], 1, 0); 
    }

    // 5. 循环创建 4 个子进程
    for (int i = 0; i < CHILD_COUNT; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork失败");
            return 1;
        } else if (pid == 0) { 
            // ================= 子进程工作区域 =================
            
            // 阻塞等待属于当前子进程的信号量
            sem_wait(&sems[i]);

            // 计算当前子进程需要读取的文件起始偏移量和大小
            off_t start = i * chunk_size;
            // 最后一个子进程负责读取剩余的所有字节（防止除不尽）
            off_t end = (i == CHILD_COUNT - 1) ? total_size : (i + 1) * chunk_size;
            off_t size_to_read = end - start;

            if (size_to_read > 0) {
                int fd = open(filename, O_RDONLY);
                if (fd >= 0) {
                    // 将文件指针移动到分配给当前子进程的起始位置
                    lseek(fd, start, SEEK_SET);

                    char buffer[4096];
                    off_t bytes_read = 0;
                    
                    // 循环读取直到读满属于自己负责的区块大小
                    while (bytes_read < size_to_read) {
                        size_t to_read = (size_to_read - bytes_read > sizeof(buffer)) ? 
                                          sizeof(buffer) : (size_to_read - bytes_read);
                        ssize_t n = read(fd, buffer, to_read);
                        if (n <= 0) break; 
                        
                        // 将读取到的内容写入到标准输出 (屏幕)
                        write(STDOUT_FILENO, buffer, n); 
                        bytes_read += n;
                    }
                    close(fd);
                } else {
                    perror("子进程打开文件失败");
                }
            }

            // 当前子进程读写完毕，唤醒下一个子进程
            if (i < CHILD_COUNT - 1) {
                sem_post(&sems[i + 1]);
            }

            // 子进程退出
            exit(0);
            // ==================================================
        }
    }

    // 6. 父进程工作区域：等待所有 4 个子进程执行完毕，防止僵尸进程
    for (int i = 0; i < CHILD_COUNT; i++) {
        wait(NULL);
    }

    // 7. 清理资源
    for (int i = 0; i < CHILD_COUNT; i++) {
        sem_destroy(&sems[i]);
    }
    munmap(sems, CHILD_COUNT * sizeof(sem_t));

    return 0;
}
