/***********************************************************
* File Name:    04getopt.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月19日 星期四 17时13分48秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    
    const char *optstring = "lhim";

    int ret = 0;

    while(1)
    {
        ret = getopt(argc,argv,optstring);
        if(ret == -1)
            break;
        switch(ret)
        {
            case 'l': printf("breakfast\n");break;
            case 'h': printf("lunch\n");break;
            case 'i': printf("dinner\n");break;
            case 'm': printf("supper\n");break;
            case '?': printf("I don't know!\n");break;
        }
    }

    return 0;
}



