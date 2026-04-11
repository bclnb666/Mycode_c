#ifndef __PORTOCOL_H
#define __PORTOCOL_H

#include <sys/types.h>
#include <sys/ipc.h>

// ftok 生成 key 需要的路径和项目 ID (这两个宏必须保证两个进程看到的一样)
// 注意：运行程序时，请确保当前目录下有权限，或者换成 "/tmp"
#define PATHNAME "." 
#define PROJ_ID  0x66

#define BUFSIZE 1024

// 为了解决同步问题，我们在共享内存中定义一个结构体
// 而不是单纯的一段字符数组
struct shm_data {
    int flag; // 同步标志位：0 代表空闲可写，1 代表有数据可读
    char buf[BUFSIZE]; // 实际存储数据的缓冲区
};

#endif
