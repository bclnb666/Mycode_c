/***********************************************************
* File Name:    05getenv.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 11时58分55秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    puts(getenv(argv[1]));


    return 0;
}
