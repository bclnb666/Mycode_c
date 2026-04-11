/***********************************************************
* File Name:    01sihnal_2.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月26日 星期四 11时47分46秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    while(1)
    {
        write(1,"~",1);
        sleep(1);
    }
    
    return 0;
}
