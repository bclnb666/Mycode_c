/***********************************************************
* File Name:    07prime_con.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 10时05分24秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MIN 100
#define MAX 300


int is_primer(int num)
{
    int i = 0;

    sleep(1);
    
    if(num <= 1)//判断num是否 <= 1
        return 0;
    if(num == 2 || num == 3)
        return 1;
    for(i = 2; i <= num / i; i++)
        if(num % i == 0)
            return 0;
    return 1;
}

int main(int argc, char *argv[]) {

    int i = 0;//循环变量
    int n = 0;//循环变量
    pid_t pid;//存储子进程的pid
    //由于wait(2)时不需要子进程的pid所以定义一个变量即可

    for(n = 0; n < 4; n++)
    {
        pid = fork();//创建子进程
        if(pid < 0)//判断创建子进程是否失败
        {
            perror("fork()");//打印错误信息
            exit(1);//由于创建子进程失败,终止进程,并且返回状态1
        }
        if(pid == 0)//判断是否是子进程
            break;
    }

    for(i = MIN; i <= MAX; i++)
    {
        if(i % 4 == n)
            if(is_primer(i))
                printf("[%d]%d is a Primer Number!\n", n, i);
    }

    //sleep(1000);

    switch(n)
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 : exit(0);
    }

    for(i = 0; i < 4; i++)//循环收尸
        wait(NULL);
    
    return 0;
}
