/***********************************************************
* File Name:    02atexit.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 10时58分49秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

void close_file(void)
{
    printf("close_file\n");
}

void free_mem(void)
{
    printf("free_mem\n");
}
void exit_begin(void)
{
    printf("exit_begin\n");
}


int main(int argc, char *argv[]) {
    
    if(atexit(close_file) != 0)
    {
        fprintf(stderr,"close_file() is failed!\n");
        exit(1);
    }

    if(atexit(free_mem) != 0)
    {
        fprintf(stderr,"free_mem() is failed!\n");
        exit(2);
    }

    if(atexit(exit_begin) != 0)
    {
        fprintf(stderr,"exit_begin() is failed!\n");
        exit(3);
    }

    printf("反对斯基奥妇女哦ifesoifjsfjfjapfjpajaaio挨打带哦风俗哦ifjsaeoi、\n");

    exit(0);
}



