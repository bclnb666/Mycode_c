/***********************************************************
* File Name:    03glob.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年03月19日 星期四 16时21分03秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

int main(int argc, char *argv[]) {
    
    glob_t gs;
    int i = 0;

    //if(glob("/etc/*",0,NULL,&gs) != 0)
    if(glob("./*",GLOB_NOSORT,NULL,&gs) != 0)
    {
        fprintf(stderr,"glob() Is Filed!\n");
        return -1;
    }
   
    //glob("/home/bcl/.*",GLOB_APPEND,NULL,&gs);
    for(i = 0; i < gs.gl_pathc; i++)
    {
        printf("%s\n",gs.gl_pathv[i]);       
    }
    globfree(&gs);


    /*if(argc < 2)
    {
        fprintf(stderr,"Usage : %s + filename\n",argv[0]);
        return -1;
    }*/

    return 0;
}


