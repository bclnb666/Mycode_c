/***********************************************************
* File Name:    mypwd.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月20日 星期五 09时42分59秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <crypt.h>

#define NAMESIZE 32

int main(int argc, char *argv[]) {

    char name[NAMESIZE] = {0};
    char *pwd = NULL;
    struct spwd *tp = NULL;
    char *cp = NULL;

    //[1]输入登陆用户名->fgets(3)
    printf("请输入用户名：");
    fgets(name,NAMESIZE,stdin);
    *strchr(name,'\n') = '\0';//把name数组中'\n'替换为'\0'

    //[2]输入密码->getpass(3)
    pwd = getpass("请输入密码：");
    if(pwd == NULL)
    {
        perror("getpass()");
        return -1;
    }
    //[3]读取真正的密码->getspnam(3)
    tp = getspnam(name);
    if(tp == NULL)
    {
       fprintf(stderr,"获取shadow文件中的用户名信息失败！\n");
       return -2;
    }
    //[4]将输入的密码进行加密->crypt(3)
    cp = crypt(pwd,tp->sp_pwdp);
    if(tp == NULL)
    {
        perror("crypt()");
        return -3;
    }
    //[5]对比密码->strcmp(3)
    if(!strcmp(tp->sp_pwdp,cp))
        printf("恭喜！登陆成功！\n");
    else
        printf("密码输入错误！\n");

    return 0;
}













