/* log.c & config.c - 按天滚动日志模块与启动热加载器 */
#include "iot_gateway.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static FILE *log_file_ptr = NULL;
static int current_log_day = -1;
static char base_log_dir = "./log/";

static void roll_log_file_if_needed(struct tm *time_info) {
    if (time_info->tm_mday!= current_log_day) {
        if (log_file_ptr) {
            fclose(log_file_ptr);
        }
        char filename;
        snprintf(filename, sizeof(filename), "%siot_gateway_%04d%02d%02d.log",
                 base_log_dir,
                 time_info->tm_year + 1900,
                 time_info->tm_mon + 1,
                 time_info->tm_mday);
                 
        log_file_ptr = fopen(filename, "a");
        if (log_file_ptr) {
            current_log_day = time_info->tm_mday;
        }
    }
}

void write_system_log(const char *level, const char *format,...) {
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    roll_log_file_if_needed(time_info);

    if (log_file_ptr) {
        fprintf(log_file_ptr, "[%04d-%02d-%02d %02d:%02d:%02d][%s] ",
                time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday,
                time_info->tm_hour, time_info->tm_min, time_info->tm_sec,
                level);

        va_list args;
        va_start(args, format);
        vfprintf(log_file_ptr, format, args);
        va_end(args);
        
        fprintf(log_file_ptr, "\n");
        fflush(log_file_ptr); /* 强制刷写内存，阻止断电引起的文件系统缓存破损 */
    }
}

/* 配置文本抽取 */
void load_system_config(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) return;
    
    char line;
    while (fgets(line, sizeof(line), fp)) {
        /* 执行基于等号分隔的配置识别引擎（省略实现细节仅表述结构概念）*/
        if (strncmp(line, "log_dir=", 8) == 0) {
            sscanf(line + 8, "%255s", base_log_dir);
        }
    }
    fclose(fp);
}
