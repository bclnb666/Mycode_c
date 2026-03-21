/***********************************************************
* File Name:    main.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月20日 星期五 14时25分40秒
***********************************************************/
#include "mycp.h"

int main(int argc, char *argv[]) {
    
    if(argc < 3)//判断命令行参数的个数是否少于3个
    {
        fprintf(stderr, "Usage : %s + srcfile + destfile\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数的个数少于3个,结束程序,并且返回-1
    }
    mycp(argv[1], argv[2]);//调用实现的内部函数,完成cp的命令功能

    return 0;
}
