#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>

#define BUFSIZE 128
#define LOCKFILE "/var/run/mydaemon.pid" // 使用普通用户可写的路径
#define OUTFILE "/tmp/out"

int lock_fd = -1; // 将锁文件描述符提升为全局变量，避免生命周期丢失
volatile sig_atomic_t running = 1; // 控制主循环的标志位

// 信号处理函数：捕获终止信号以实现优雅退出
static void sig_handler(int signo)
{
    if (signo == SIGTERM || signo == SIGINT) {
        running = 0;
    }
}

// 标准的 System V 守护进程实现 (Double-Fork)
static int mydaemon(void)
{   
    pid_t pid;
    int fd;
    
    // [1] 第一次 fork
    pid = fork();
    if(pid < 0) return -1;
    if(pid > 0) exit(0); // 父进程退出
    
    // [2] 创建新会话，脱离控制终端
    if(setsid() == (pid_t)-1) return -2;
    
    // [3] 第二次 fork (防止进程再次打开控制终端)
    signal(SIGHUP, SIG_IGN); // 忽略 SIGHUP 信号
    pid = fork();
    if(pid < 0) return -1;
    if(pid > 0) exit(0); // 第一次 fork 的子进程退出
    
    // [4] 重设文件权限掩码
    umask(0);
    
    // [5] 切换工作路径到根目录
    if(chdir("/") == -1) return -3;
    
    // [6] 关闭/重定向标准输入、输出、错误到 /dev/null
    fd = open("/dev/null", O_RDWR);
    if(fd >= 0)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if(fd > 2) close(fd);
    }

    return 0;
}

// 进程单实例互斥锁
static int process_already_running(void)
{
    char buf[BUFSIZE] = {0};

    // 使用 O_RDWR 方便后续截断，权限设为 0644
    lock_fd = open(LOCKFILE, O_RDWR | O_CREAT, 0644);
    if(lock_fd < 0)
    {
        syslog(LOG_ERR, "无法打开锁文件 %s: %s", LOCKFILE, strerror(errno));
        return -1;
    }

    // 尝试获取非阻塞排他锁
    if(flock(lock_fd, LOCK_EX | LOCK_NB) == -1)
    {
        syslog(LOG_WARNING, "程序已经运行，无法获取互斥锁。");
        close(lock_fd);
        return -2;
    }

    // 成功获取锁，清空文件内容并写入新的 PID
    if(ftruncate(lock_fd, 0) == -1)
    {
        syslog(LOG_ERR, "截断锁文件失败: %s", strerror(errno));
        return -3;
    }

    snprintf(buf, BUFSIZE, "%d\n", getpid());
    if(write(lock_fd, buf, strlen(buf)) != (ssize_t)strlen(buf))
    {
        syslog(LOG_ERR, "写入 PID 到锁文件失败: %s", strerror(errno));
        return -4;
    }

    // 此时绝对不能关闭 lock_fd，否则锁会失效
    return 0;
}

int main(void)
{
    FILE *fp = NULL;
    time_t tm;
    struct tm *time_st = NULL;
    char buf[BUFSIZE] = {0};

    // 开启 syslog，标识为 "mydaemon"，包含 PID
    openlog("mydaemon", LOG_PID | LOG_NDELAY, LOG_DAEMON);

    // 注册信号处理函数
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);

    // 1. 初始化守护进程
#if 1
    if(mydaemon() < 0) // 使用优化后的 Double-Fork 自定义函数
    {
        syslog(LOG_ERR, "mydaemon() 初始化失败");
        exit(EXIT_FAILURE);
    }
#else
    if(daemon(0, 0) == -1) // 系统自带函数，参数均设为 0
    {
        syslog(LOG_ERR, "daemon() 初始化失败: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
#endif

    // 2. 检查单实例运行状态
    if(process_already_running() < 0)
    {
        exit(EXIT_FAILURE); // 如果已经运行或报错，直接退出
    }

    syslog(LOG_INFO, "守护进程启动成功");

    // 3. 打开目标输出文件
    fp = fopen(OUTFILE, "a"); // 改为 "a" (追加模式) 更符合日志或持续写入的逻辑
    if(fp == NULL)
    {
        syslog(LOG_ERR, "打开输出文件 %s 失败: %s", OUTFILE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 4. 核心工作循环
    while(running)
    {
        if(time(&tm) == (time_t)-1)
        {
            syslog(LOG_ERR, "获取时间戳失败");
            break; 
        }

        time_st = localtime(&tm);
        if(time_st == NULL)
        {
            syslog(LOG_ERR, "转换本地时间失败");
            break;
        }

        strftime(buf, BUFSIZE, "%Y年%m月%d日 %H:%M:%S\n", time_st);
        fputs(buf, fp);
        fflush(fp); // 精确刷新指定的文件流
        
        sleep(1);
    }

    // 5. 优雅退出与资源清理
    syslog(LOG_INFO, "收到终止信号，正在清理资源并退出...");
    if(fp) fclose(fp);
    if(lock_fd >= 0)
    {
        // 删除锁文件并关闭描述符
        unlink(LOCKFILE); 
        close(lock_fd);
    }
    closelog();

    return 0;
}
