/* token_bucket.c - 惰性求值数学模型与令牌桶限流算法 */
#include "iot_gateway.h"
#include <time.h>
#include <pthread.h>

struct rate_limiter_bucket {
    double capacity;         /* 容纳的峰值爆发配额 */
    double available_tokens; /* 现存瞬时许可权 */
    double refill_rate;      /* 每秒钟允许新注入的配额比例 */
    struct timespec last_eval; /* 上一次进行惰性推演的高精度时间戳 */
    pthread_mutex_t lock;    /* 并发校验环境中的内存屏障锁 */
};

static struct rate_limiter_bucket global_udp_bucket;

/* 从系统内核获取连续且不受时区重置影响的单调时标秒数 */
static double get_monotonic_timestamp(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

void token_bucket_init(double max_capacity, double tokens_per_second) {
    global_udp_bucket.capacity = max_capacity;
    global_udp_bucket.available_tokens = max_capacity; /* 初始化处于最高防爆发准备 */
    global_udp_bucket.refill_rate = tokens_per_second;
    clock_gettime(CLOCK_MONOTONIC, &global_udp_bucket.last_eval);
    pthread_mutex_init(&global_udp_bucket.lock, NULL);
}

/* 核心：非阻塞配额索取 (返回 1 表示合法可放行，0 表示超出配额应拦截) */
int token_bucket_consume_nonblocking(double required_tokens) {
    pthread_mutex_lock(&global_udp_bucket.lock);

    double current_time = get_monotonic_timestamp();
    double last_time = global_udp_bucket.last_eval.tv_sec + 
                      (double)global_udp_bucket.last_eval.tv_nsec / 1000000000.0;
    
    /* 时间流逝推演 */
    double elapsed_time = current_time - last_time;

    /* 基于惰性求值计算本应累积的新生令牌 */
    global_udp_bucket.available_tokens += elapsed_time * global_udp_bucket.refill_rate;
    
    /* 严格切削超出物理上限的残余份额 */
    if (global_udp_bucket.available_tokens > global_udp_bucket.capacity) {
        global_udp_bucket.available_tokens = global_udp_bucket.capacity;
    }

    int request_granted = 0;
    if (global_udp_bucket.available_tokens >= required_tokens) {
        /* 放行数据报，扣除对应许可额度 */
        global_udp_bucket.available_tokens -= required_tokens;
        request_granted = 1;
    }

    /* 将校验锚点推进至当前时刻，用于下一次数据碰撞的增量判定 */
    clock_gettime(CLOCK_MONOTONIC, &global_udp_bucket.last_eval);
    
    pthread_mutex_unlock(&global_udp_bucket.lock);
    return request_granted;
}
