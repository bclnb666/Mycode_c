/***********************************************************
* File Name:    test.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月28日 星期六 16时50分09秒
***********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    // 注意：这里printf没有加换行符\n
    printf("before fork");
    fork();
    printf("after fork\n");
    return 0;
}
