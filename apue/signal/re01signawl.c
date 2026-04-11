/***********************************************************
* File Name:    re01signawl.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月28日 星期六 10时03分10秒
***********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void handler(int none)//注册的新行为(当SIGINT信号到来了,会调用handler函数)
{
    write(1, "!", 1);//往标准输出写一个"!"
}

int main(void)
{
    struct sigaction oldact,newact;


    /*if((signal(SIGINT, handler)) == SIG_ERR)//判断给SIGINT信号设置新的行为是否失败
    {
        perror("signal()");//打印错误信息
        return -1;//由于给SIGINT信号设置新的行为失败,结束程序,并且返回-1
    }*/
    
    newact.sa_handler = handler;
    sigemptyset(&newact.sa_mask);
    sigaddset(&newact.sa_mask,SIGINT);
    sigaction(SIGINT,&newact,NULL);

    while(1)//死循环,每1s往标准输出写一个"*"
    {
        write(1, "*", 1);//往标准输出写一个"*"
        sleep(1);//睡1s
    }

    return 0;
}

