/***********************************************************
* File Name:    mybash.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月24日 星期二 18时34分03秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define LNAMESIZE 128
#define HNAMESIZE 128
#define PWDSIZE   256
#define BUFSIZE   16

static int get_env_pwd(char *lname,char *hname,char *pwd)
{
    char *tempname = NULL;
    if((tempname = getenv("LOGNAME")) == NULL)
        return -1;
    else
        strcpy(lname,tempname);

    if(gethostname(hname,HNAMESIZE) < 0)
    {
        perror("gethostname()");
        return -2;
    }
    else
        strcpy(lname,tempname);

    if(getcwd(pwd,PWDSIZE) == NULL)
    {
        perror("getcwd()");
        return -3;
    }
    
    return 0;
}

static int get_cmd_line(char **line,size_t *length)
{
    if(getline(line,length,stdin) == -1)
    {
        perror("getline()");
        return -1;
    }
    return 0;
}

static int parse_string(char *str,char *delim,char **buf,int size)
{
    int i = 0;
    while(i < size)
    {
        buf[i] = strtok(str,delim);
        if(buf[i] == NULL)
            break;
        i++;
        str = NULL;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    
    char lname[LNAMESIZE] = {0};
    char hname[HNAMESIZE] = {0};
    char pwd[PWDSIZE] = {0};
    char *line = NULL;
    size_t length = 0;
    char *buf[BUFSIZE] = {0};
    pid_t pid;

    while(1)
    {
        if(get_env_pwd(lname,hname,pwd) < 0)
            exit(1);
        else
            printf("%s@%s:%s$ ",lname,hname,pwd);

        if(get_cmd_line(&line,&length) < 0)
            exit(2);

        if(!strcmp(line,"exit\n"))
            break;
        

        parse_string(line," \n",buf,BUFSIZE);

        pid = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(3);
        }
        if(pid == 0)
        {
            execvp(buf[0],buf);
            perror("execvp()");
            exit(4);
        }
        else
            wait(NULL);

    }
    free(line);
    return 0;
}










