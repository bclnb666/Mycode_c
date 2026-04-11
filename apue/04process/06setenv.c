/***********************************************************
* File Name:    06setenv.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月23日 星期一 14时28分30秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    setenv("TEST","ARE YOU OK?",1);
    puts(getenv("TEST"));

    return 0;
}
