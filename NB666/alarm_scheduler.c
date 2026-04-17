/* alarm_scheduler.c - 链表升序虚拟调度引擎与多重闹钟模拟 */
#include "iot_gateway.h"
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/* 定时事件调度单元 */
typedef struct scheduled_task {
    void (*callback_function)(void);  /* 到期自动执行的业务逻辑 */
    long interval_ms;                 /* 触发周期（若为0则是单次延迟型任务）*/
    long next_execution_time_ms;      /* 核算所得的未来绝对到期基准点 */
    struct scheduled_task *next;
} scheduled_task_t;

static scheduled_task_t *scheduler_list_head = NULL;

/* 提取进程运行系统物理时间戳转化为毫秒 */
static long get_current_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/* 优先队列策略：基于触发的绝对时间将任务挂载进入链表适当位置 */
static void insert_task_ordered(scheduled_task_t *new_task) {
    /* 链表为空，或新任务最为紧迫应置于首位 */
    if (!scheduler_list_head |

| new_task->next_execution_time_ms < scheduler_list_head->next_execution_time_ms) {
        new_task->next = scheduler_list_head;
        scheduler_list_head = new_task;
        return;
    }
    
    /* 游标滑动，寻找时序空档 */
    scheduled_task_t *curr = scheduler_list_head;
    while (curr->next && curr->next->next_execution_time_ms <= new_task->next_execution_time_ms) {
        curr = curr->next;
    }
    new_task->next = curr->next;
    curr->next = new_task;
}

/* 提取首节点，测算时间差，重配底层硬件定时器 */
static void rearm_hardware_timer(void) {
    if (!scheduler_list_head) return;

    long now = get_current_time_ms();
    long delta = scheduler_list_head->next_execution_time_ms - now;
    if (delta <= 0) delta = 1; /* 补偿延误，立即促发中断响应 */

    struct itimerval itv;
    itv.it_value.tv_sec = delta / 1000;
    itv.it_value.tv_usec = (delta % 1000) * 1000;
    /* 放弃内核的循环，交由本逻辑处理器的软件重部署完成循环，规避重叠冲突 */
    itv.it_interval.tv_sec = 0; 
    itv.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, NULL);
}

/* 由内核中断层挂钩并自动触发的主回调网闸 */
void scheduler_alarm_isr_handler(int signo) {
    if (signo!= SIGALRM) return;
    
    long now = get_current_time_ms();
    
    /* 循环收割所有在本次时间轴上超期的悬留任务 */
    while (scheduler_list_head && scheduler_list_head->next_execution_time_ms <= now) {
        scheduled_task_t *expired_task = scheduler_list_head;
        scheduler_list_head = expired_task->next;

        if (expired_task->callback_function) {
            expired_task->callback_function();
        }

        if (expired_task->interval_ms > 0) {
            /* 赋予新的生命周期并重新编队 */
            expired_task->next_execution_time_ms = now + expired_task->interval_ms;
            insert_task_ordered(expired_task);
        } else {
            /* 阅后即焚任务内存释放 */
            free(expired_task);
        }
    }
    rearm_hardware_timer();
}

/* 提供至业务应用层的公共注入接口 */
void scheduler_add_task(void (*callback)(void), long delay_ms, long interval_ms) {
    scheduled_task_t *task = malloc(sizeof(scheduled_task_t));
    if (!task) return;
    
    task->callback_function = callback;
    task->interval_ms = interval_ms;
    task->next_execution_time_ms = get_current_time_ms() + delay_ms;
    
    /* 操作全局链表时实施屏蔽，防止 SIGALRM 中断造成脏写截断数据链 */
    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);
    
    insert_task_ordered(task);
    rearm_hardware_timer();
    
    sigprocmask(SIG_SETMASK, &old_mask, NULL);
}
