#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "gateway.h"
#include "config.h"
#include "logger.h"
#include "udp_server.h"
#include "token_bucket.h"
#include "ipc_modules.h" 
// [新增] 引入线程池头文件
#include "thread_pool.h" 

#define BUFFER_SIZE 1024

// 声明外部的全局线程池对象 (在 main.c 中实例化)
extern threadpool_t *g_threadpool;

// 1. 定义跨线程传递的任务参数结构体
typedef struct {
    char buffer[BUFFER_SIZE];
    char sender_ip[32]; // 【注意线程安全】多线程不要直接共用 inet_ntoa，用字符串固定存下来
} udp_task_args_t;

int init_udp_server(void) {
    int sock;
    struct sockaddr_in local_addr;
    struct ip_mreq mreq;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG_ERR("[UDP-服务] Socket 创建失败");
        return -1;
    }

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG_ERR("[UDP-服务] 设置 SO_REUSEADDR 失败");
        close(sock);
        return -1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(g_config.multicast_port);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        LOG_ERR("[UDP-服务] 绑定端口 %d 失败", g_config.multicast_port);
        close(sock);
        return -1;
    }

    mreq.imr_multiaddr.s_addr = inet_addr(g_config.multicast_ip);
    
    if (strlen(g_config.local_ip) == 0) {
        strcpy(g_config.local_ip, "0.0.0.0");
    }

    if (strcmp(g_config.local_ip, "0.0.0.0") == 0) {
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    } else {
        mreq.imr_interface.s_addr = inet_addr(g_config.local_ip);
    }
    
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        LOG_ERR("[UDP-服务] 加入组播组失败: %m (IP: %s)", g_config.local_ip);
        close(sock);
        return -1;
    }

    LOG_INFO("[UDP-服务] 非阻塞组播服务初始化成功! 监听: %s:%d (fd=%d)", 
             g_config.multicast_ip, g_config.multicast_port, sock);
             
    return sock; 
}

// 2. 线程池工作线程真正执行的业务函数！(完全脱离了内核的中断和 poll 阻塞)
static void process_udp_data_task(void *arg) {
    // 强转回我们定义的参数结构体
    udp_task_args_t *task_args = (udp_task_args_t *)arg;
    
    uint32_t sensor_id = 0;
    double sensor_data = 0.0;
    
    // 开始尝试解析合并的 JSON 格式
    char *temp_str = strstr(task_args->buffer, "\"temp\":");
    char *hum_str = strstr(task_args->buffer, "\"hum\":");
    
    if (temp_str != NULL && hum_str != NULL) {
        double temp_val = 0.0, hum_val = 0.0;
        
        sscanf(temp_str + 7, "%lf", &temp_val);
        sscanf(hum_str + 6, "%lf", &hum_val);
        
        // --- 提取成功，将【温度】推入队列 ---
        pthread_mutex_lock(&queue_mutex);
        int next_head = (queue_head + 1) % MAX_QUEUE_SIZE;
        if (next_head != queue_tail) {
            data_queue[queue_head].sensor_id = 16790;
            data_queue[queue_head].sensor_data = temp_val;
            queue_head = next_head;
            pthread_mutex_unlock(&queue_mutex);
            sem_post(&data_sem); 
            
            update_shared_sensor_data(16790, temp_val);
            LOG_INFO("[UDP-处理线程] 解析合并格式成功(%s) -> 温度: %.1f", task_args->sender_ip, temp_val);
        } else {
            pthread_mutex_unlock(&queue_mutex);
        }

        // --- 提取成功，将【湿度】推入队列 ---
        pthread_mutex_lock(&queue_mutex);
        next_head = (queue_head + 1) % MAX_QUEUE_SIZE;
        if (next_head != queue_tail) {
            data_queue[queue_head].sensor_id = 16789;
            data_queue[queue_head].sensor_data = hum_val;
            queue_head = next_head;
            pthread_mutex_unlock(&queue_mutex);
            sem_post(&data_sem); 
            
            update_shared_sensor_data(16789, hum_val);
            LOG_INFO("[UDP-处理线程] 解析合并格式成功(%s) -> 湿度: %.1f", task_args->sender_ip, hum_val);
        } else {
            pthread_mutex_unlock(&queue_mutex);
        }
    }
    // 兼容解析旧的单条数据格式
    else if (sscanf(task_args->buffer, "%u,%lf", &sensor_id, &sensor_data) == 2 ||
        sscanf(task_args->buffer, "%u %lf", &sensor_id, &sensor_data) == 2 ||
        sscanf(task_args->buffer, "{\"id\":%u, \"data\":%lf}", &sensor_id, &sensor_data) == 2) 
    {
        pthread_mutex_lock(&queue_mutex);
        int next_head = (queue_head + 1) % MAX_QUEUE_SIZE;
        if (next_head != queue_tail) {
            data_queue[queue_head].sensor_id = sensor_id;
            data_queue[queue_head].sensor_data = sensor_data;
            queue_head = next_head;
            pthread_mutex_unlock(&queue_mutex);
            sem_post(&data_sem); 
            
            update_shared_sensor_data(sensor_id, sensor_data);
            
            LOG_INFO("[UDP-处理线程] 接管下位机(%s)单条数据: ID=%u, Value=%.2f", 
                     task_args->sender_ip, sensor_id, sensor_data);
        } else {
            pthread_mutex_unlock(&queue_mutex);
            LOG_WARN("[UDP-处理线程] 队列已满，丢弃下位机数据");
        }
    } else {
        LOG_WARN("[UDP-处理线程] 收到未知格式内容来自 %s: %s", 
                 task_args->sender_ip, task_args->buffer);
    }

    // 3. 【极度关键】任务执行完后，必须由工人线程释放这块内存，否则内存泄露！
    free(task_args); 
}

// 被 poll 主循环直接调用的读取事件回调函数 (要求极速返回！)
void handle_udp_read(int sock) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    
    // 动作一：仅仅收数据
    int n = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, 
                     (struct sockaddr *)&sender_addr, &sender_len);
                     
    if (n <= 0) return;
    buffer[n] = '\0'; 

    // 动作二：极速前置过滤 (防限流拦截要在分配内存之前，节省性能)
    if (token_bucket_consume(1) == 0) {
        LOG_WARN("[限流器] UDP rate limit exceeded, dropping packet from %s", inet_ntoa(sender_addr.sin_addr));
        return;
    }

    // 动作三：打包装箱，丢给后方线程池！
    // 动态分配内存，防止因为异步执行导致原本在栈上的变量被覆盖
    udp_task_args_t *args = (udp_task_args_t *)malloc(sizeof(udp_task_args_t));
    if (args == NULL) {
        LOG_ERR("[UDP-事件] 内存分配失败，丢弃数据");
        return;
    }
    
    // 拷贝数据
    strncpy(args->buffer, buffer, BUFFER_SIZE - 1);
    args->buffer[BUFFER_SIZE - 1] = '\0';
    // 在主线程这里转换 IP 地址最为安全，防止多线程 inet_ntoa 导致的数据紊乱
    strncpy(args->sender_ip, inet_ntoa(sender_addr.sin_addr), sizeof(args->sender_ip) - 1);
    args->sender_ip[sizeof(args->sender_ip) - 1] = '\0';

    // 投递任务到线程池
    if (g_threadpool != NULL) {
        if (threadpool_add(g_threadpool, process_udp_data_task, args) != 0) {
            LOG_WARN("[UDP-事件] 线程池队列已满，任务堆积，丢弃该数据包");
            free(args); // 如果投递失败，主线程顺手回收内存
        }
    } else {
        // 如果线程池未就绪的安全兜底
        free(args);
    }
    
    // 主线程立即返回，继续去 poll 里监控其余百万并发！
}
