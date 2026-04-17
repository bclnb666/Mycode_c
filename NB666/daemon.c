/* daemon.c - 守护进程化与单例锁机制实现 */
#include "iot_gateway.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define PID_FILE_PATH "/var/run/iot_gateway.pid"

/* 
 * 基于 fcntl 的文件排他锁机制
 * 确保同一台设备上只能运行一个网关实例
 */
static int lock_pid_file(int fd) {
    struct flock fl;
    fl.l_type = F_WRLCK;     /* 排他性写锁 */
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;            /* 锁定整个文件 */
    return fcntl(fd, F_SETLK, &fl);
}

void daemonize_process(void) {
    pid_t pid;

    /* 第一次 fork，终止父进程 */
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    /* 创建新会话，脱离控制终端 */
    if (setsid() < 0) exit(EXIT_FAILURE);

    /* 忽略 SIGHUP 信号，防止终端断开时发送的挂断信号杀死进程 */
    signal(SIGHUP, SIG_IGN);

    /* 第二次 fork，彻底剥夺重新获取控制终端的能力 */
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    /* 重置文件权限掩码，确保守护进程创建文件的权限完全由代码控制 */
    umask(0);

    /* 将工作目录切换至根目录，防止占用可卸载文件系统 */
    if (chdir("/") < 0) exit(EXIT_FAILURE);

    /* 遍历并关闭所有从父进程继承的打开的文件描述符 */
    long max_fd = sysconf(_SC_OPEN_MAX);
    for (long fd = 0; fd < max_fd; fd++) {
        close(fd);
    }

    /* 将标准输入、输出、错误重定向至 /dev/null */
    int fd0 = open("/dev/null", O_RDWR);
    int fd1 = dup(0);
    int fd2 = dup(0);
    (void)fd0; (void)fd1; (void)fd2; /* 抑制编译器未使用变量警告 */

    /* 创建并锁定 PID 文件 */
    int pid_fd = open(PID_FILE_PATH, O_RDWR | O_CREAT, 0640);
    if (pid_fd < 0) {
        exit(EXIT_FAILURE);
    }
    
    if (lock_pid_file(pid_fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            /* 无法获取排他锁，表明另一实例正在运行 */
            exit(EXIT_FAILURE);
        }
    }
    
    /* 清空文件内容并写入当前进程PID */
    if (ftruncate(pid_fd, 0) < 0) exit(EXIT_FAILURE);
    char pid_str[1];
    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    if (write(pid_fd, pid_str, strlen(pid_str))!= (ssize_t)strlen(pid_str)) {
        exit(EXIT_FAILURE);
    }
    
    /* 注意：故意不关闭 pid_fd，内核将在进程退出时自动释放文件锁 */
}
