#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <errno.h>
#include "gateway.h"
#include "logger.h"
#include "udp_server.h"
#include "io_multiplexing.h"
#include "daemon.h"

#define MAX_POLL_FDS 1024

void start_event_loop(int udp_sock) {
    struct pollfd fds[MAX_POLL_FDS];
    int nfds = 0;

    // 初始化所有槽位
    for (int i = 0; i < MAX_POLL_FDS; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    }

    // 1. 仅注册 UDP 监听套接字
    if (udp_sock >= 0) {
        fds[nfds].fd = udp_sock;
        fds[nfds].events = POLLIN;
        nfds++;
    }

    LOG_INFO("==== 网关异步事件驱动引擎 (poll) 已全面接管 (纯净 UDP 模式) ====");

    while (g_running) {
        int ready = poll(fds, nfds, -1);
        
        if (ready < 0) {
            // 捕获被信号 (如 SIGALRM 闹钟) 中断的正常现象
            if (errno == EINTR) {
                continue; // 忽略中断，重新进入 poll 阻塞监听
            }
            if (g_running == 0) break; 
            LOG_ERR("[主循环] poll 系统调用出错");
            break;
        }

        // 动态遍历所有发生了事件的 Socket
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                
                // 【事件 1】：UDP 组播收到了真实下位机数据
                if (fds[i].fd == udp_sock) {
                    handle_udp_read(udp_sock);
                }
                
                // 移除了原先的 IPC 连接和数据读取事件
            }
        }
    }
    
    LOG_INFO("[主循环] 收到退出信号，事件引擎安全停机.");
}
