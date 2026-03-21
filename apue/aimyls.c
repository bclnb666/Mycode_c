/***********************************************************
* File Name:    aimyls.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月19日 星期四 20时04分45秒
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define BUFSIZE 10
#define TIMELEN 32

/* ==========================================
 * 以下是获取文件详细属性的辅助函数 (用于 -l)
 * ========================================== */

/**
 * @brief 获取文件类型的字符表示
 * @param st_mode 文件元信息中的 st_mode 成员
 */
static char get_file_type(mode_t st_mode) {
    char c = 0;
    // 使用 S_IFMT 掩码提取文件类型，并通过 switch 判断具体类型
    switch(st_mode & S_IFMT) {
        case S_IFREG : c = '-'; break; // 普通文件
        case S_IFDIR : c = 'd'; break; // 目录
        case S_IFCHR : c = 'c'; break; // 字符设备
        case S_IFBLK : c = 'b'; break; // 块设备
        case S_IFSOCK: c = 's'; break; // 套接字
        case S_IFIFO : c = 'p'; break; // 管道
        case S_IFLNK : c = 'l'; break; // 符号链接
        default      : c = '?'; break; // 未知类型
    }
    return c;
}

/**
 * @brief 获取文件的权限字符串
 * @param st_mode 文件元信息中的 st_mode 成员
 * @param buf 用于存储结果的字符数组
 */
static char *get_file_permission(mode_t st_mode, char *buf) {
    // 定义 9 个权限位的掩码，依次代表：属主/属组/其他的 读/写/执行 权限
    int mask[BUFSIZE - 1] = {S_IRUSR, S_IWUSR, S_IXUSR,
                             S_IRGRP, S_IWGRP, S_IXGRP,
                             S_IROTH, S_IWOTH, S_IXOTH};
    char permission[BUFSIZE] = "rwxrwxrwx"; // 初始预设为满权限
    int i = 0;

    for(i = 0; i < BUFSIZE - 1; i++) {
        // 如果 st_mode 中不包含该掩码对应的权限，则将该位修改为 '-'
        if(!(st_mode & mask[i]))
            permission[i] = '-';
    }
    strncpy(buf, permission, BUFSIZE);
    return buf;
}

/**
 * @brief 根据 UID 获取所属者的用户名
 */
static char *get_file_uname(uid_t st_uid) {
    // getpwuid 解析 /etc/passwd 获取用户信息
    struct passwd *p = getpwuid(st_uid);
    if(p == NULL) {
        // 若找不到对应用户（如用户已被删），直接返回数字形式的 UID
        static char uid_str[32];
        snprintf(uid_str, sizeof(uid_str), "%d", st_uid);
        return uid_str;
    }
    return p->pw_name;
}

/**
 * @brief 根据 GID 获取所属组的组名
 */
static char *get_file_gname(gid_t st_gid) {
    // getgrgid 解析 /etc/group 获取组信息
    struct group *p = getgrgid(st_gid);
    if(p == NULL) {
        // 若找不到对应组名，直接返回数字形式的 GID
        static char gid_str[32];
        snprintf(gid_str, sizeof(gid_str), "%d", st_gid);
        return gid_str;
    }
    return p->gr_name;
}

/**
 * @brief 将时间戳转换为格式化的时间字符串
 */
static char *get_file_mtime(time_t tm, char *tbuf) {
    // localtime 将时间戳转换为本地时间（结构体）
    struct tm *p = localtime(&tm);
    if(p == NULL) {
        strcpy(tbuf, "Unknown Time");
        return tbuf;
    }
    // strftime 格式化时间，类似于 sprintf
    strftime(tbuf, TIMELEN, "%m月 %d %H:%M", p);
    return tbuf;
}

/**
 * @brief 将字节大小转换为带单位的人类可读格式 (用于 -h 选项)
 * @param size 文件的字节大小
 * @param buf 用于存储转换后字符串的缓冲区
 */
static char *format_size(off_t size, char *buf) {
    const char *units[] = {"B", "K", "M", "G", "T"}; // 单位阶梯
    int i = 0;
    double s = size;

    // 只要数值大于等于1024，且单位还没超出 T，就一直除以 1024 进位
    while (s >= 1024 && i < 4) {
        s /= 1024;
        i++;
    }

    if (i == 0) {
        // 如果是 Byte 级别，不需要小数点，直接打印整数
        snprintf(buf, 16, "%ld", (long)size);
    } else {
        // 如果是 K, M, G, T 级别，保留一位小数并附上单位
        snprintf(buf, 16, "%.1f%s", s, units[i]);
    }
    return buf;
}

/* ==========================================
 * 核心显示逻辑
 * ========================================== */

/**
 * @brief 打印单个文件的信息
 * @param full_path 完整路径，用于 lstat 提取真实属性
 * @param display_name 仅用于在屏幕上显示的文件名
 * @param opt_l 标记是否启用了 -l (长格式)
 * @param opt_i 标记是否启用了 -i (显示 inode)
 * @param opt_h 标记是否启用了 -h (人性化显示大小)
 */
void display_info(const char *full_path, const char *display_name, int opt_l, int opt_i, int opt_h) {
    struct stat fs;

    // 只要启用了 -i (需要 inode) 或者 -l (需要全面属性)，我们就必须调用 lstat
    if (opt_i || opt_l) {
        // 使用 lstat 获取软链接本身的属性，而不是它指向的源文件
        if (lstat(full_path, &fs) == -1) {
            perror("lstat()");
            return;
        }
    }

    // --- 短格式输出处理 (无 -l 选项时) ---
    if (!opt_l) {
        if (opt_i) {
            // 如果只有 -i 没有 -l，在文件名前打印 inode 号
            printf("%7ld ", (long)fs.st_ino);
        }
        printf("%s  ", display_name); // 横向打印文件名
        return;
    }

    // --- 以下为长格式输出处理 (-l 选项启用时) ---
    char perms[BUFSIZE] = {0};
    char timebuf[TIMELEN] = {0};
    char sizebuf[32] = {0};

    // 【处理 -h 选项】：转换文件大小
    if (opt_h) {
        format_size(fs.st_size, sizebuf); // 转换为 1.2K 这种形式
    } else {
        snprintf(sizebuf, sizeof(sizebuf), "%ld", (long)fs.st_size); // 保持字节数
    }

    // 【处理 -i 选项】：如果在长格式中也要求显示 inode，打印在最前面
    if (opt_i) {
        printf("%7ld ", (long)fs.st_ino);
    }

    // 格式化输出长信息 (类型权限 链接数 属主 属组 大小 时间 文件名)
    // %8s 保证大小那一列有足够的宽度对齐
    printf("%c%s %3ld %-8s %-8s %8s %s %s\n",
        get_file_type(fs.st_mode),
        get_file_permission(fs.st_mode, perms),
        (long)fs.st_nlink,
        get_file_uname(fs.st_uid),
        get_file_gname(fs.st_gid),
        sizebuf,  // 这里传入刚才处理好的 sizebuf (字节数或带单位的字符串)
        get_file_mtime(fs.st_mtime, timebuf),
        display_name
    );
}

/**
 * @brief 遍历目录并打印其下的所有内容
 */
void display_dir(const char *path, int opt_l, int opt_a, int opt_i, int opt_h) {
    // 打开目录流
    DIR *dp = opendir(path);
    if (dp == NULL) {
        perror(path);
        return;
    }

    struct dirent *entry;
    // 循环读取目录中的目录项 (dirent)
    while ((entry = readdir(dp)) != NULL) {
        // 【处理 -a 选项】：如果没有 -a 参数，且文件名以 '.' 开头，则跳过隐藏文件
        if (!opt_a && entry->d_name[0] == '.') {
            continue;
        }

        // 拼接出子文件的完整相对路径/绝对路径供 lstat 获取属性
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // 调用核心打印函数输出该子项的信息
        display_info(full_path, entry->d_name, opt_l, opt_i, opt_h);
    }

    // 短格式输出下，全部文件横向打印结束后，补一个换行符
    if (!opt_l) {
        printf("\n");
    }

    // 关闭目录流
    closedir(dp);
}

int main(int argc, char *argv[]) {
    // 用于记录各个选项是否开启的标志位
    int opt_l = 0, opt_a = 0, opt_i = 0, opt_d = 0, opt_h = 0;
    int ch;

    // 1. 使用 getopt 解析命令行选项
    // "laidh" 代表支持这些字母开头的短选项，且都不带参数
    while ((ch = getopt(argc, argv, "laidh")) != -1) {
        switch (ch) {
            case 'l': opt_l = 1; break;
            case 'a': opt_a = 1; break;
            case 'i': opt_i = 1; break;
            case 'd': opt_d = 1; break;
            case 'h': opt_h = 1; break;
            case '?':
                fprintf(stderr, "Usage: %s [-laidh] [file/dir...]\n", argv[0]);
                return -1;
        }
    }

    // 2. 分析余下的非选项参数 (即目标文件或目录的路径)
    // optind 是 getopt 留下来的索引，代表第一个不是选项的参数的下标
    if (optind == argc) {
        // 情况A：没有给定具体的路径参数，默认为当前目录 "."
        if (opt_d) {
            // 如果加了 -d，仅显示当前目录本身的属性，不需要打开遍历它
            display_info(".", ".", opt_l, opt_i, opt_h);
            if (!opt_l) printf("\n"); // 短格式补一个换行
        } else {
            // 否则，遍历当前目录内容
            display_dir(".", opt_l, opt_a, opt_i, opt_h);
        }
    } else {
        // 情况B：给定了一个或多个目标路径参数
        for (int j = optind; j < argc; j++) {
            struct stat fs;
            // 探查该参数是普通文件还是目录
            if (lstat(argv[j], &fs) == -1) {
                perror(argv[j]); // 查无此文件时报错并继续
                continue;
            }

            // 【处理 -d 选项的核心逻辑】：
            // 如果目标是一个目录，并且「没有」指定 -d 选项，才去遍历它的内容
            if (S_ISDIR(fs.st_mode) && !opt_d) {
                // 如果用户给出了多个路径参数，打印目录名作为抬头区分
                if (argc - optind > 1) {
                    printf("%s:\n", argv[j]);
                }
                display_dir(argv[j], opt_l, opt_a, opt_i, opt_h);

                // 长格式下，多个目录的输出结果之间空一行排版
                if (j < argc - 1 && opt_l) {
                    printf("\n");
                }
            } else {
                // 如果是普通文件，或者哪怕是目录但用户指定了 -d (当作普通文件对待)
                // 直接打印它的属性即可
                display_info(argv[j], argv[j], opt_l, opt_i, opt_h);
                if (!opt_l) {
                    printf("\n");
                }
            }
        }
    }

    return 0;
}
