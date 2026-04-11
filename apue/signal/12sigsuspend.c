/***********************************************************
* File Name:    12sigsuspend.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月27日 星期五 14时27分35秒
***********************************************************/


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler(int none)
{
    write(1, "!", 1);//往标准输出文件中写一个!
}

int main(void)
{
    int i = 0, j = 0;//循环变量
   

    struct sigaction oldact,newact;
    oldact.sa_handler = SIG_DFL;
    sigemptyset(&oldact.sa_mask);
    oldact.sa_flags = 0;
    newact.sa_handler = sig_handler;
    sigemptyset(&newact.sa_mask);
    sigaddset(&newact.sa_mask,SIGINT);
    sigaddset(&newact.sa_mask,SIGRTMIN);
    newact.sa_flags = 0;
    
    sigaction(SIGINT,&newact,NULL);
    sigaction(SIGRTMIN,&newact,NULL);

    //sigemptyset(&new);//把new信号集清空
   // sigaddset(&new, SIGINT);//将SIGINT信号添加到new信号集中
   // sigaddset(&new, SIGRTMIN);//将SIGRTMIN信号添加到new信号集中

    signal(SIGINT, sig_handler);//给SIGINT信号设置新的行为
    signal(SIGRTMIN, sig_handler);//给SIGRTMIN信号设置新的行为

    //给当前调用进程阻塞new信号集中的信号,并且把原有的信号屏蔽字存储到old中
    sigprocmask(SIG_BLOCK, &newact.sa_mask, &oldact.sa_mask);

    //在定义每一行内容期间不被SIGINT信号干扰
    for(i = 0; i < 10; i++)
    {
        //给当前调用进程阻塞new信号集中的信号,并且把原有的信号屏蔽字存储到old中
        //sigprocmask(SIG_BLOCK, &new, &old);
        for(j = 0; j < 5; j++)
        {
            write(1, "*", 1);//往标准输出文件中写一个"*"
            sleep(1);//睡1s
        }
        write(1, "\n", 1);//往标准输出文件中写一个"\n"
        //sigprocmask(SIG_SETMASK, &old, NULL);
        //给当前调用进程恢复之前的信号屏蔽字
        sigsuspend(&oldact.sa_mask);//当执行这一条语句相当于在执行
        /*当执行这一条语句相当于在执行
        [1]sigprocmask(SIG_SETMASK, &old, &set);
        [2]pause();
        [3]sigprocmask(SIG_SETMASK, &set, NULL);
        属于原子操作
        */
    }

    return 0;
}


