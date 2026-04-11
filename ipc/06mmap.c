/***********************************************************
* File Name:    06mmap.c
* Author:       Amon
* mail:         bamon1026@gmail.com
* Created Time: 2026年04月01日 星期三 09时38分36秒
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define LENGTH 1024

int main(int argc, char *argv[]) {

    void *ptr = NULL;
    int ret = 0;
    pid_t pid;
        
    ptr = mmap(NULL,LENGTH,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap()");
        ret = -1;
        goto ERR_1;
    }
    pid = fork();
    if(pid == -1)
    {
        perror("fork()");
        ret = -2;
        goto ERR_2;
    }
    if(pid == 0)
    {
       memcpy(ptr,"Praisethefool!",14);
       munmap(ptr,LENGTH);
       exit(0);
    }
    wait(NULL);
    puts(ptr);

    
ERR_2:
    munmap(ptr,LENGTH);
ERR_1:
    return ret;
}
