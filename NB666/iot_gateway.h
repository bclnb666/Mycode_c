/* iot_gateway.h - 嵌入式物联网设备管理平台全局统一头文件 */
#ifndef IOT_GATEWAY_H
#define IOT_GATEWAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/* 模块: 守护进程化与单例控制 (daemon.c)                                      */
/* -------------------------------------------------------------------------- */
/**
 * 将当前进程转为后台守护进程，并在 /var/run 下挂载排他锁防多开
 */
void daemonize_process(void);


/* -------------------------------------------------------------------------- */
/* 模块: 异步非阻塞多路复用网络框架 (io_multiplexing.c)                       */
/* -------------------------------------------------------------------------- */
/**
 * 启动网关核心的事件轮询器，监听外部TCP指令接入与UDP传感器数据报文
 * @param tcp_port 供上游管理云平台调用的 TCP 端口
 * @param udp_port 供下游传感器上报数据的 UDP 端口
 */
void enter_io_event_loop(int tcp_port, int udp_port);


/* -------------------------------------------------------------------------- */
/* 模块: 线程池与并发任务处理 (thread_pool.c)                                 */
/* -------------------------------------------------------------------------- */
struct thread_pool;

/**
 * 预先构建执行后台任务的并发线程池
 * @param num_threads 初始化线程的数量规模
 * @return 成功返回线程池指针，失败返回NULL
 */
struct thread_pool *thread_pool_create(int num_threads);

/**
 * 将任务推入无锁化等待队列并唤醒沉睡的工作线程
 * @param pool 已初始化的线程池对象
 * @param function 处理业务的函数指针
 * @param argument 传递给业务函数的参数内存指针
 * @return 0 为成功，-1为失败
 */
int thread_pool_add_task(struct thread_pool *pool, void (*function)(void *), void *argument);

/**
 * 安全广播所有线程退出，并在完成收尾工作后销毁线程池及释放其内存
 */
void thread_pool_destroy(struct thread_pool *pool);


/* -------------------------------------------------------------------------- */
/* 模块: System V 进程间通信与共享内存控制 (ipc_modules.c)                    */
/* -------------------------------------------------------------------------- */
/**
 * 创建并获取内核级的共享内存段及保证互斥的 System V 信号量数组
 * @return 0为成功，-1为失败
 */
int ipc_system_init(void);

/**
 * 向共享内存区域写入传感器获取的高频时序数据，该操作受信号量严密保护
 * @param raw_data 传感器原始数据流
 */
void publish_sensor_data_to_shm(const char *raw_data);

/**
 * 清除所有注册在内核表中的 IPC 通信资源
 */
void ipc_system_cleanup(void);


/* -------------------------------------------------------------------------- */
/* 模块: 高阶非阻塞令牌桶限流器 (token_bucket.c)                              */
/* -------------------------------------------------------------------------- */
/**
 * 建立数学计算限流模型
 * @param max_capacity   最大突发流量配额 (峰值缓冲)
 * @param tokens_per_second 每秒恒定注入系统的放行凭证率
 */
void token_bucket_init(double max_capacity, double tokens_per_second);

/**
 * 对进项数据进行超低延迟非阻塞裁决
 * @param required_tokens 本次操作需剥夺的令牌数
 * @return 1表示放行，0表示超出阈值应当立即丢弃
 */
int token_bucket_consume_nonblocking(double required_tokens);


/* -------------------------------------------------------------------------- */
/* 模块: 基于链表的集中式时序时钟引擎 (alarm_scheduler.c)                     */
/* -------------------------------------------------------------------------- */
/**
 * 面向操作系统开放的中断响应函数，捕获并处理系统闹钟信号 (SIGALRM)
 * @param signo 捕获到的信号代码
 */
void scheduler_alarm_isr_handler(int signo);

/**
 * 注入一个未来的定时任务节点到调度链表中
 * @param callback  到期后回调触发的函数地址
 * @param delay_ms  初次执行所需延缓的毫秒数
 * @param interval_ms 再次循环的周期(如果为0表示只运行单次)
 */
void scheduler_add_task(void (*callback)(void), long delay_ms, long interval_ms);


/* -------------------------------------------------------------------------- */
/* 模块: OS 级身份权限验证模块 (password_auth.c)                              */
/* -------------------------------------------------------------------------- */
/**
 * 基于 shadow 散列密码文件进行的身份鉴权（需赋予特权环境）
 * @param req_user 发起请求的系统用户名
 * @param req_pass 明文密码
 * @return 0表示通过，-1表示拒绝
 */
int shadow_system_authenticate(const char *req_user, const char *req_pass);


/* -------------------------------------------------------------------------- */
/* 模块: 全局日志审计与配置解析 (log.c & config.c)                            */
/* -------------------------------------------------------------------------- */
/**
 * 将系统内部运行状态和报警信息输出，内置基于日期的自动滚动切分特性
 * @param level 警告等级 (如 "INFO", "ERROR")
 * @param format C标准格式化可变参字符串
 */
void write_system_log(const char *level, const char *format,...);

/**
 * 提取纯文本配置属性并映射至系统变量
 * @param filepath 配置文件所在的磁盘路径
 */
void load_system_config(const char *filepath);

#ifdef __cplusplus
}
#endif

#endif /* IOT_GATEWAY_H */
