/***********************************************************
* File Name:    mystat.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月18日 星期三 17时23分35秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>  // open 函数需要此头文件

#define BUFSIZE 10
#define SIZE 8192   // 扩大缓冲区以一次性读完常见的 passwd 文件

struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};
struct group {
    char   *gr_name;        /* group name */
    char   *gr_passwd;      /* group password */
    gid_t   gr_gid;         /* group ID */
    char  **gr_mem;         /* NULL-terminated array of pointers
                               to names of group members */
};

int myKMP(char *buf, char *needle);
static int *get_next(const char *needle, int len);
static struct passwd *mygetpwuid(uid_t st_uid);
static struct group *mygetgrgid(gid_t st_gid);

static char get_file_type(mode_t st_mode)
{
    char c = 0;
    switch(st_mode & S_IFMT)
   {
        case S_IFREG : c = '-';break;
        case S_IFDIR : c = 'd';break;
        case S_IFCHR : c = 'c';break;
        case S_IFBLK : c = 'b';break;
        case S_IFSOCK : c = 's';break;
        case S_IFIFO : c = 'p';break;
        case S_IFLNK : c = 'l';break;
        default : c = '?';break;
    }
    return c;
}

static char *get_file_permission(mode_t st_mode,char *buf)
{
    int mask[BUFSIZE - 1] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};
    char permission[BUFSIZE] = "rwxrwxrwx";
    for(int i = 0; i < BUFSIZE - 1; i++)
    {
        if(!(st_mode & mask[i]))
            permission[i] = '-';
    }

    strncpy(buf,permission,BUFSIZE);
    return buf;
}

static int get_file_nlink(nlink_t st_nlink)
{
    return st_nlink;
}

static char *get_file_uname(uid_t st_uid)
{
    struct passwd *p = NULL;
    p = mygetpwuid(st_uid);
    if(p == NULL)
    {
        // 找不到用户时，返回uid的字符串形式更稳妥
        static char err_uid[32];
        sprintf(err_uid, "%d", st_uid);
        return err_uid;
    }
    
    return p->pw_name;
}

static char *get_file_gname(gid_t st_gid)
{
    struct group *g = NULL;
    g = mygetgrgid(st_gid);
    if(g == NULL)
    {
        static char err_gid[32];
        sprintf(err_gid, "%d", st_gid);
        return err_gid;
    }
    return g->gr_name;
}

int main(int argc, char *argv[]) {
    
    struct stat fs;
    char buf[BUFSIZE] = {0};
    if(argc < 2)
    {
        fprintf(stderr,"Usage : %s + filename\n",argv[0]);
        return -1;
    }

    if(stat(argv[1],&fs) == -1)
    {
        perror("stat()");
        return -2;
    }

    printf("%c",get_file_type(fs.st_mode));
    printf("%s",get_file_permission(fs.st_mode,buf));
    printf(" %d",get_file_nlink(fs.st_nlink));
    printf(" %s",get_file_uname(fs.st_uid));
    printf(" %s\n",get_file_gname(fs.st_gid));
    return 0;
}

static struct passwd *mygetpwuid(uid_t st_uid)
{
    // 使用 static 防止返回局部变量的指针
    static struct passwd p;
    static char name[32]; 
    p.pw_name = name;

    int fd = open("/etc/passwd", O_RDONLY); // 修复: 增加双引号
    char buf[SIZE] = {0};

    if(fd < 0)
    {
        perror("open()");
        return NULL;
    }
    
    // 对于一般 /etc/passwd，一次性读完最简单稳妥
    int count = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    if (count <= 0) return NULL;

    // 修复: 必须将 uid 转换为字符串
    // 技巧：搜索 ":1000:" 确保匹配的是 UID 字段，防止与用户名或路径中的数字混淆
    char uid_str[32] = {0};
    sprintf(uid_str, ":%d:", st_uid);
    
    int flag = myKMP(buf, uid_str);
    if (flag == -1) return NULL; // 未找到

    // 修复: 安全提取用户名，从查找到的位置往前找回该行的行首
    int line_start = flag;
    while(line_start > 0 && buf[line_start - 1] != '\n') {
        line_start--;
    }

    // 从行首向后读取，直到遇到第一个 ':'
    int j = 0;
    while(buf[line_start] != ':' && buf[line_start] != '\0' && j < sizeof(name) - 1) {
        name[j++] = buf[line_start++];
    }
    name[j] = '\0';

    return &p;
}

static struct group *mygetgrgid(gid_t st_gid)
{
    // 使用 static 防止返回局部变量的指针
    static struct group g;
    static char name[32]; 
    g.gr_name = name;

    int fd = open("/etc/group", O_RDONLY); // 修复: 增加双引号
    char buf[SIZE] = {0};

    if(fd < 0)
    {
        perror("open()");
        return NULL;
    }
    
    // 对于一般 /etc/group，一次性读完最简单稳妥
    int count = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    if (count <= 0) return NULL;

    // 修复: 必须将 uid 转换为字符串
    // 技巧：搜索 ":1000:" 确保匹配的是 UID 字段，防止与用户名或路径中的数字混淆
    char gid_str[32] = {0};
    sprintf(gid_str, ":%d:", st_gid);
    
    int flag = myKMP(buf, gid_str);
    if (flag == -1) return NULL; // 未找到

    // 修复: 安全提取用户名，从查找到的位置往前找回该行的行首
    int line_start = flag;
    while(line_start > 0 && buf[line_start - 1] != '\n') {
        line_start--;
    }

    // 从行首向后读取，直到遇到第一个 ':'
    int j = 0;
    while(buf[line_start] != ':' && buf[line_start] != '\0' && j < sizeof(name) - 1) {
        name[j++] = buf[line_start++];
    }
    name[j] = '\0';

    return &g;
}
static int *get_next(const char *needle,int len)
{
    int *next = malloc(len * sizeof(int));
    if(!next) return NULL;
    
    next[0] = 0;
    if(len <= 1)
        return next;
    
    int j = 0;
    for(int i = 1; i < len; i++)
    {
        while(j > 0 && needle[i] != needle[j])
            j = next[j - 1];
        if(needle[i] == needle[j])
            j++;
        next[i] = j;
    }
    
    return next;
}

int myKMP(char *buf, char *needle)
{
    // 修复: 指针传入时 sizeof 获取的是指针大小，必须用 strlen
    int buf_len = strlen(buf);
    int needle_len = strlen(needle);
    
    int *next = get_next(needle, needle_len);
    if (!next) return -1;

    int i = 0, j = 0;

    for(; i < buf_len; i++)
    {
        while(j > 0 && buf[i] != needle[j])
            j = next[j - 1];
        if(buf[i] == needle[j])
            j++;
        if(j == needle_len)
        {
            free(next); // 修复: 释放 malloc 的内存，防止内存泄露
            return (i - needle_len + 1);
        }
    }

    free(next); // 修复: 未找到时也要释放内存
    return -1;    
}
