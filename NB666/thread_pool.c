/* thread_pool.c - POSIX 多线程池与条件变量任务调度队列 */
#include "iot_gateway.h"
#include <stdlib.h>
#include <pthread.h>

/* 定义无锁化设计风格的链表任务节点 */
typedef struct thread_task {
    void (*function)(void *);    /* 执行函数的函数指针 */
    void *argument;              /* 上下文数据包传递指针 */
    struct thread_task *next;
} thread_task_t;

struct thread_pool {
    pthread_mutex_t lock;        /* 保护共享队列资源的互斥锁 */
    pthread_cond_t notify;       /* 任务到达时的唤醒条件变量 */
    pthread_t *threads;          /* 工作线程句柄数组 */
    thread_task_t *queue_head;   /* 任务队列头节点 */
    thread_task_t *queue_tail;   /* 任务队列尾节点 */
    int thread_count;            /* 线程池固定规模容量 */
    int queue_size;              /* 当前堆积的等待任务总数 */
    int shutdown;                /* 服务销毁关闭状态标识 */
};

/* 工作线程核心常驻例程 */
static void *thread_worker_routine(void *pool_ctx) {
    struct thread_pool *pool = (struct thread_pool *)pool_ctx;

    for (;;) {
        pthread_mutex_lock(&pool->lock);

        /* 
         * 基于 while 循环的条件等待防线，彻底防御条件变量的“伪唤醒”现象。
         * 当队列为空且尚未收到销毁指令时，线程持续让出 CPU 挂起。
         */
        while ((pool->queue_size == 0) && (!pool->shutdown)) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        /* 捕获系统退出指令，工作线程主动结束生命周期 */
        if (pool->shutdown && pool->queue_size == 0) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        /* 安全出队：提取任务并更新链表指针 */
        thread_task_t *task = pool->queue_head;
        pool->queue_size--;
        if (pool->queue_size == 0) {
            pool->queue_head = NULL;
            pool->queue_tail = NULL;
        } else {
            pool->queue_head = task->next;
        }

        /* 在执行耗时任务前立即释放锁，保证其他工作线程可以同时出队新任务 */
        pthread_mutex_unlock(&pool->lock);

        /* 执行注册的业务回调，运行完毕后销毁任务载体 */
        (*(task->function))(task->argument);
        free(task);
    }
    return NULL;
}

struct thread_pool *thread_pool_create(int num_threads) {
    if (num_threads <= 0) return NULL;

    struct thread_pool *pool = malloc(sizeof(struct thread_pool));
    if (!pool) return NULL;

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->shutdown = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&(pool->threads[i]), NULL, thread_worker_routine, (void *)pool);
    }
    return pool;
}

int thread_pool_add_task(struct thread_pool *pool, void (*function)(void *), void *argument) {
    if (!pool ||!function) return -1;

    pthread_mutex_lock(&pool->lock);

    /* 可根据需要通过配置判断 pool->queue_size 是否达到最大容量边界来限制内存增长 */

    thread_task_t *new_task = malloc(sizeof(thread_task_t));
    new_task->function = function;
    new_task->argument = argument;
    new_task->next = NULL;

    if (pool->queue_size == 0) {
        pool->queue_head = new_task;
        pool->queue_tail = new_task;
    } else {
        pool->queue_tail->next = new_task;
        pool->queue_tail = new_task;
    }
    pool->queue_size++;

    /* 信号唤醒：向操作系统申请唤醒一个沉睡的可用工作线程 */
    pthread_cond_signal(&pool->notify); 
    pthread_mutex_unlock(&pool->lock);
    
    return 0;
}

void thread_pool_destroy(struct thread_pool *pool) {
    if (!pool) return;

    pthread_mutex_lock(&pool->lock);
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->lock);
        return;
    }
    pool->shutdown = 1;
    /* 广播唤醒所有线程池内的休眠线程，使其察觉 shutdown 状态并退出 */
    pthread_cond_broadcast(&pool->notify); 
    pthread_mutex_unlock(&pool->lock);

    /* 同步阻塞等待所有子线程处理完最后的事务并安全退出 */
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    /* 回收残留的未执行任务链表节点，阻止内存泄漏 */
    thread_task_t *curr = pool->queue_head;
    while (curr!= NULL) {
        thread_task_t *next = curr->next;
        free(curr);
        curr = next;
    }

    free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool);
}
