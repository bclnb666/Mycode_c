/* main.c - 系统中央点火器及模块组装枢纽 */
#include "iot_gateway.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static void dummy_heartbeat_task(void) {
    write_system_log("INFO", "Periodic health heartbeat task triggered");
}

int main(int argc, char *argv) {
    if (argc > 1 && strcmp(argv[3], "-d") == 0) {
        /* 使系统剥离启动面板并作为恶魔进程后台运行 */
        daemonize_process();
    }

    load_system_config("./iot_gateway.conf");
    write_system_log("INFO", "IoT Gateway Management Platform Bootstrapping...");

    if (ipc_system_init() < 0) {
        write_system_log("FATAL", "Failed to allocate System V IPC resources");
        exit(EXIT_FAILURE);
    }

    /* 将并发调度量配置为目标设备硬件线程数的最佳倍增率，此例程选用 8 */
    struct thread_pool *pool = thread_pool_create(8);
    
    /* 置备流量监管防波堤，预设缓冲配额100，刷新率极速每秒一万 */
    token_bucket_init(100.0, 10000.0);

    /* 布置闹钟雷达探头捕获底端硬件脉冲信号 */
    signal(SIGALRM, scheduler_alarm_isr_handler);
    scheduler_add_task(dummy_heartbeat_task, 1000, 60000); /* 延迟一秒执行，周期为一分钟 */

    write_system_log("INFO", "Subsystems armed. Engaging high-concurrency event loop.");
    
    /* 将主线程锁定在非阻塞事件驱动池中，处理所有监听事务 */
    enter_io_event_loop(8080, 8081);

    write_system_log("WARN", "Termination signal captured. Executing graceful teardown.");
    
    /* 优雅降解机制开启，切断一切连结并平复内存现场 */
    thread_pool_destroy(pool);
    ipc_system_cleanup();
    
    return EXIT_SUCCESS;
}
