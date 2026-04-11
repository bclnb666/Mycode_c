/***********************************************************
* File Name:    07setjmp.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 15时38分25秒
***********************************************************/
#include <stdio.h>
//#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

int div(int num1,int num2)
{
    if(num2 == 0)
    {
        longjmp(env,1);
    }
    return num1 / num2;
}

int main(int argc, char *argv[]) {

    int num1 = 0, num2 = 0;
    int sum = 0;

    if(setjmp(env) == 0)
        printf("清输入两个整数：");
    else
        printf("清从新输入刘盎格整形术：（注意出书不能呢个为0）");
    scanf("%d %d",&num1,&num2);
    sum = div(num1,num2);
    printf("%d / %d = %d\n",num1,num2,sum);

    return 0;
}
