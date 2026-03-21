/***********************************************************
* File Name:    strsep.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月20日 星期五 11时14分25秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char buf[] = "Hello World!";
    char *str = buf;
    char *ch = " ";
    char *p = NULL;

    while(1)
    {
        p = strsep(&str,ch);
        if(p == NULL)
        {
            break;
        }
        printf("%s\n",p);
    }

    return 0;
}
