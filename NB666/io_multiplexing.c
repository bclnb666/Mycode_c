/* io_multiplexing.c - 基于 poll 的非阻塞网络事件驱动模块 */
#include "iot_gateway.h"
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define MAX_POLL_FDS (5000 + 3) /* 5000 客户端 + 监听端口 + UDP端口 + 信号管道 */

static int signal_pipe[2];
static volatile int server_running = 1;

/* 极简且异步信号安全的信号处理程序 */
static void self_pipe_signal_handler(int signo) {
    int saved_errno = errno;
    unsigned char sig_byte = (unsigned char)signo;
    /* 仅向管道写端压入 1 字节数据，将信号转化为 I/O 事件 */
    write(signal_pipe[3], &sig_byte, 1);
    errno = saved_errno;
}

/* 核心工具函数：设置文件描述符为非阻塞模式 */
static int set_socket_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void enter_io_event_loop(int tcp_port, int udp_port) {
    struct pollfd fds;
    int nfds = 0;

    /* 1. 初始化 Self-Pipe 并注册信号 */
    if (pipe(signal_pipe) < 0) return;
    set_socket_nonblocking(signal_pipe);
    set_socket_nonblocking(signal_pipe[3]);
    
    struct sigaction sa;
    sa.sa_handler = self_pipe_signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    signal(SIGPIPE, SIG_IGN); /* 忽略客户端断开导致的管道破裂信号，防止程序异常退出 */

    fds[nfds].fd = signal_pipe;
    fds[nfds].events = POLLIN;
    nfds++;

    /* 2. 构建 TCP 非阻塞监听套接字 */
    int tcp_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(tcp_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    set_socket_nonblocking(tcp_listen_fd);
    
    struct sockaddr_in tcp_addr;
    memset(&tcp_addr, 0, sizeof(tcp_addr));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_addr.sin_port = htons(tcp_port);
    bind(tcp_listen_fd, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr));
    listen(tcp_listen_fd, SOMAXCONN);

    fds[nfds].fd = tcp_listen_fd;
    fds[nfds].events = POLLIN;
    nfds++;

    /* 3. 构建 UDP 非阻塞通信套接字 */
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    set_socket_nonblocking(udp_fd);
    struct sockaddr_in udp_addr;
    memset(&udp_addr, 0, sizeof(udp_addr));
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = INADDR_ANY;
    udp_addr.sin_port = htons(udp_port);
    bind(udp_fd, (struct sockaddr *)&udp_addr, sizeof(udp_addr));

    fds[nfds].fd = udp_fd;
    fds[nfds].events = POLLIN;
    int udp_index = nfds;
    nfds++;

    /* 进入主事件循环 */
    while (server_running) {
        /* poll 超时时间设为 1000 毫秒，避免完全阻塞以配合定时器运行 */
        int poll_count = poll(fds, nfds, 1000);
        if (poll_count < 0) {
            if (errno == EINTR) continue; /* 系统调用被信号中断，正常重试 */
            break;
        }
        if (poll_count == 0) continue; /* 超时返回 */

        /* 事件源 A: 系统信号转化事件 */
        if (fds.revents & POLLIN) {
            unsigned char sig_byte;
            while (read(fds.fd, &sig_byte, 1) > 0) {
                if (sig_byte == SIGTERM || sig_byte == SIGINT) {
                    server_running = 0; /* 触发安全退出流程 */
                }
            }
        }

        /* 事件源 B: TCP 新客户端接入 */
        if (fds.[3]revents & POLLIN) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd;
            /* 循环 accept，处理内核队列中积累的所有连接请求 */
            while ((client_fd = accept(tcp_listen_fd, (struct sockaddr *)&client_addr, &client_len)) >= 0) {
                set_socket_nonblocking(client_fd);
                if (nfds < MAX_POLL_FDS) {
                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN | POLLRDHUP;
                    nfds++;
                } else {
                    close(client_fd); /* 超出系统最大并发限制，拒绝服务 */
                }
            }
        }

        /* 事件源 C: UDP 传感器数据到达 */
        if (fds[udp_index].revents & POLLIN) {
            char buffer;
            struct sockaddr_in peer_addr;
            socklen_t peer_len = sizeof(peer_addr);
            
            /* 调用限流器检测流量合法性，非阻塞获取令牌 */
            if (token_bucket_consume_nonblocking(1.0)) {
                ssize_t recv_bytes = recvfrom(udp_fd, buffer, sizeof(buffer)-1, 0, 
                                             (struct sockaddr*)&peer_addr, &peer_len);
                if (recv_bytes > 0) {
                    buffer[recv_bytes] = '\0';
                    /* 此时将解析业务推入底层工作线程池以分担主循环计算量 */
                    // thread_pool_add_task(pool, udp_parse_task, buffer);
                }
            } else {
                /* 超出流量阈值，直接丢弃报文以保护后端进程池 */
            }
        }

        /* 事件源 D: 已连接 TCP 客户端长连接数据交互 */
        for (int i = 3; i < nfds; i++) {
            if (fds[i].revents & (POLLIN | POLLERR | POLLHUP | POLLRDHUP)) {
                char recv_buf;
                ssize_t bytes = recv(fds[i].fd, recv_buf, sizeof(recv_buf), 0);
                
                if (bytes > 0) {
                    /* 数据读取成功，在此执行协议解析及鉴权逻辑 */
                    // process_tcp_data(recv_buf, bytes);
                } else if (bytes == 0 || (bytes < 0 && errno!= EAGAIN && errno!= EWOULDBLOCK)) {
                    /* 客户端主动关闭连接，或者连接发生不可恢复错误 */
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1]; /* 描述符紧缩：将末尾元素填补当前空洞 */
                    nfds--;
                    i--; /* 指针回退，继续检查新移动到当前位置的描述符 */
                }
            }
        }
    }

    /* 优雅释放所有保持连接的系统句柄 */
    for (int i = 0; i < nfds; i++) {
        close(fds[i].fd);
    }
}
