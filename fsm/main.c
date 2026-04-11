#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h> // [新增] 引入epoll头文件

#include "fsm.h"

#define EPOLL

#define TTY1    "/dev/tty9"
#define TTY2    "/dev/tty10"

int main(void)
{
    fsm_t *fsm12 = NULL; // 指向r9w10的有限状态机
    fsm_t *fsm21 = NULL; // 指向r10w9的有限状态机
    int fd1, fd2;        // 存储打开文件的文件描述符

    fd1 = open(TTY1, O_RDWR); // 打开/dev/tty9设备
    if(fd1 == -1)
    {
        perror("open()");
        return -1;
    }
    write(fd1, "[****tty9****]", 14);

    fd2 = open(TTY2, O_RDWR | O_NONBLOCK); // 打开/dev/tty10设备
    if(fd2 == -1)
    {
        perror("open()");
        close(fd1);
        return -2;
    }
    write(fd2, "[!!!tty10!!!]", 13);

    fsm_init(&fsm12, fd1, fd2);
    fsm_init(&fsm21, fd2, fd1);

#if defined(EPOLL)
    // [步骤1] 创建 epoll 实例 (呼叫器)
    int epfd = epoll_create(1); 
    if(epfd == -1)
    {
        perror("epoll_create()");
        return -1;
    }

    struct epoll_event ev;        // 用于临时设置事件的结构体
    struct epoll_event events[2]; // 托盘：专门用来接收内核返回的就绪事件

    // [步骤2-1] 把 fd1 和 fd2 初始添加到 epoll 监听列表 (ADD)
    // 初始化时不监听任何事件(events=0)，后续在while循环内根据状态机动态修改
    ev.data.fd = fd1;
    ev.events = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev); 

    ev.data.fd = fd2;
    ev.events = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev); 
#endif

    // 推动有限状态机的运行
    while(fsm12->state != STATE_T && fsm21->state != STATE_T)
    {
#if defined(EPOLL)
        // 如果状态到达E态(异常)，不需要监听，直接推状态机让它打印错误并进入T态
        if(fsm12->state == STATE_E)
        {
            fsm_drive(fsm12);
            continue;
        }
        if(fsm21->state == STATE_E)
        {
            fsm_drive(fsm21);
            continue;
        }

        uint32_t ev1 = 0; // 记录本次循环中 fd1 需要监听的事件
        uint32_t ev2 = 0; // 记录本次循环中 fd2 需要监听的事件

        // 根据 fsm12(r9w10) 的状态，设置需要的事件
        if(fsm12->state == STATE_R) ev1 |= EPOLLIN;  // 想读fd1
        if(fsm12->state == STATE_W) ev2 |= EPOLLOUT; // 想写fd2

        // 根据 fsm21(r10w9) 的状态，设置需要的事件
        if(fsm21->state == STATE_R) ev2 |= EPOLLIN;  // 想读fd2
        if(fsm21->state == STATE_W) ev1 |= EPOLLOUT; // 想写fd1

        // [步骤2-2] 动态修改(MOD) epoll 中需要监听的事件
        ev.data.fd = fd1;
        ev.events = ev1;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev); 

        ev.data.fd = fd2;
        ev.events = ev2;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev); 

        // [步骤3] 阻塞等待就绪事件
        int n = epoll_wait(epfd, events, 2, -1);
        if(n == -1)
        {
            if(errno == EINTR) // 判断是否被信号打断
                continue;
            perror("epoll_wait()");
            goto ERR_1;
        }

        // 用两个标志位记录本次需不需要推动对应的状态机
        int drive_fsm12 = 0;
        int drive_fsm21 = 0;

        // 遍历托盘里被内核填好的 n 个就绪事件
        for(int i = 0; i < n; i++)
        {
            if(events[i].data.fd == fd1) // 如果是 fd1 就绪了
            {
                if(events[i].events & EPOLLIN)  drive_fsm12 = 1; // fd1可读，推动 fsm12(r9w10)去读
                if(events[i].events & EPOLLOUT) drive_fsm21 = 1; // fd1可写，推动 fsm21(r10w9)去写
            }
            if(events[i].data.fd == fd2) // 如果是 fd2 就绪了
            {
                if(events[i].events & EPOLLIN)  drive_fsm21 = 1; // fd2可读，推动 fsm21(r10w9)去读
                if(events[i].events & EPOLLOUT) drive_fsm12 = 1; // fd2可写，推动 fsm12(r9w10)去写
            }
        }

        // 根据托盘返回的结果，精准推动对应的状态机
        if(drive_fsm12) fsm_drive(fsm12);
        if(drive_fsm21) fsm_drive(fsm21);
#else
        fsm_drive(fsm12);
        fsm_drive(fsm21);
#endif
    }
ERR_1:
    fsm_destroy(fsm12);
    fsm_destroy(fsm21);

    close(fd2);
    close(fd1);
#if defined(EPOLL)
    close(epfd); // 最后别忘了关闭 epoll 实例的文件描述符
#endif

    return 0;
}
