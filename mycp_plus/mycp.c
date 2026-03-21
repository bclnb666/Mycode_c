/***********************************************************
* File Name:    mycp.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月20日 星期五 14时21分51秒
***********************************************************/
#include "mycp.h"

int file_cp(const char *src, const char *dest)
{
    FILE *fps = NULL;
    FILE *fpd = NULL;
    
    char buf[BUFSIZE] = {0};
    size_t bytes_read; // 用于记录实际读到的字节数

    // 注意：加入 "b" 表示以二进制模式打开（"rb", "wb"）
    // 虽然在 Linux 系统下 "r" 和 "rb" 没区别，但这是一种好习惯，保证跨平台安全
    fps = fopen(src, "rb");
    if(fps == NULL)
    {
        perror("fopen() src");
        return -1;
    }

    fpd = fopen(dest, "wb");
    if(fpd == NULL)
    {
        perror("fopen() dest");
        fclose(fps);
        return -2;
    }

    // 修复致命问题：使用 fread 和 fwrite 代替 fgets/fputs 以支持二进制文件
    while(1)
    {
        // 从源文件读取数据到 buf 中
        bytes_read = fread(buf, 1, BUFSIZE, fps);
        
        // 如果读到的字节数为 0，说明到了文件末尾或者出错了
        if(bytes_read == 0) {
            break;
        }
        
        // 将实际读到的字节数写入到目标文件
        fwrite(buf, 1, bytes_read, fpd);
    }
    
    fclose(fpd);
    fclose(fps);
    return 0;
}

// 专门用于递归拷贝目录的函数
int dir_copy_recursive(const char *src_dir, const char *dest_dir)
{
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    struct stat statbuf;

    // 获取源目录的权限
    if (stat(src_dir, &statbuf) == -1) {
        perror("stat() src_dir");
        return -1;
    }

    // 创建目标目录 (如果已存在则忽略错误)
    if (mkdir(dest_dir, statbuf.st_mode & 0777) == -1) {
        if (errno != EEXIST) { // EEXIST 代表文件已存在
            perror("mkdir() dest_dir");
            return -1;
        }
    }

    // 打开源目录流
    dp = opendir(src_dir);
    if (dp == NULL) {
        perror("opendir() src");
        return -1;
    }

    // 循环读取目录内容
    while ((entry = readdir(dp)) != NULL) {
        // 忽略特殊的当前目录(.)和上一级目录(..)
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 拼接出完整的源路径和目标路径
        char src_path[2048];
        char dest_path[2048];
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

        // 递归调用 mycp 总控函数
        mycp(src_path, dest_path);
    }

    closedir(dp);
    return 0;
}

// 命令总控入口函数
int mycp(const char *src, const char *dest)
{
    struct stat src_stat;

    // 读取源文件的状态信息
    if (stat(src, &src_stat) == -1) {
        perror("stat() src");
        return -1;
    }

    // 通过 S_ISDIR 宏判断它是不是一个目录
    if (S_ISDIR(src_stat.st_mode)) {
        return dir_copy_recursive(src, dest);
    } else {
        // 不是目录就当作普通文件来拷贝
        return file_cp(src, dest);
    }
}
