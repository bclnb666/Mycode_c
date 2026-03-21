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
#include <pwd.h>
#include <grp.h>
#include <time.h>


#define BUFSIZE 10 
#define TIMELEN 32

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
    p = getpwuid(st_uid);
    if(p == NULL)
    {
        perror("getpwuid()");
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

static int get_file_size(off_t st_size)
{
    return st_size;
}

static char *get_file_mtime(time_t tm,char *tbuf)
{
    struct tm *p = NULL;
    p = localtime(&tm);
    if(p == NULL)
    {
        perror("localtime()");
        return NULL;
    }
    strftime(tbuf,TIMELEN,"%m月 %d %H:%M",p);
    return tbuf;
}

int main(int argc, char *argv[]) {
    
    struct stat fs;
    char buf[BUFSIZE] = {0};
    char tbuf[TIMELEN] = {0};
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
    printf("  %d",get_file_nlink(fs.st_nlink));
    printf(" %s",get_file_uname(fs.st_uid));
    printf(" %s",get_file_gname(fs.st_gid));
    printf("  %d",get_file_size(fs.st_size));
    printf("  %s",get_file_mtime(fs.st_mtime,tbuf));
    printf(" %s\n",argv[1]);
    return 0;
}


