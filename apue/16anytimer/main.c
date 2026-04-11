#include <stdio.h>
#include <unistd.h>
#include "anytimer.h"

void sig_handler(void *arg)
{
    write(1,arg,1);
}

int main()
{
    anytimer_init(10,sig_handler,(void *)"!");
    anytimer_init(1,sig_handler,(void *)"!");
    anytimer_init(5,sig_handler,(void *)"!");

    while(1)
    {
        write(1,"*",1);
        sleep(1);
    }


    return 0;
}
