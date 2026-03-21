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
//#include <pwd.h>
#include <grp.h>

#define BUFSIZE 10

#define SIZE 4096

struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

int myKMP(char *buf,char *needle);

static int *get_next(const char *needle,int len);

static struct passwd *mygetpwuid(uid_t st_uid);



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
        perror("mygetpwuid()");
        return NULL;
    }
    
    return p->pw_name;
}
static char *get_file_gname(gid_t st_gid)
{
    struct group *g = NULL;
    g = getgrgid(st_gid);
    if(g == NULL)
    {
        perror("getgrgid()");
        return NULL;
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
    struct passwd *p;
    int fd = open(/etc/passwd,O_RDONLY);
    
    char buf[SIZE] = {0};

    if(fd < 0)
        {
            perror("open()");
            return NULL;
        }
    int count = 0;

    while(1)
    {
        memset(buf,0,sizeof(buf));
        count = read(fd,buf,sizeof(buf));
        if(count == 0)
            break;
        else if(count < 0)
        {
            perror("read()");
            return NULL;
        }
        write(fd,buf,count);
    }
    
    int flag = myKMP(buf,(char *)st_uid);
    char name[10] = {0};
    int j = 0;
    while(buf[flag--] != '\n');
    flag +=2;
    while(buf[flag++] != ':')
        name[j++] = buf[flag];
    name[j] = '\0';
    memcpy(p->pw_name,&name,sizeof(name)/sizeof(name[0]));

    return p;


}

static int *get_next(const char *needle,int len)
{
    //初始化
    int *next = malloc(len * sizeof(int));
    next[0] = 0;
    if(len <= 1)
        return next;
    
    int j = 0;
    
    for(int i = 1;i < len; i++)
    {
        //判断字串前后缀不相等的情况
        while(j > 0 && needle[i] != needle[j])
            j = next[j - 1];
        //相等的情况
        if(needle[i] == needle[j])
            j++;
        //更新next数组下标
        next[i] = j;
    }
    
    return next;

}

int myKMP(char *buf,char *needle)
{
    int buf_len = sizeof(buf);
    int needle_len = sizeof(needle);
    
    int *next = get_next(needle,needle_len);

    int i = 0, j = 0;

    for(; i < buf_len; i++)
    {
        while(j > 0 && buf[i] != needle[j])
            j = next[j - 1];
        if(buf[i] == needle[j])
            j++;
        if(j == needle_len)
            return (i - needle_len + 1);
    }

    return -1;    
}






