#include "tbf.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#define TBFMAX 1024

typedef struct tbf_st
{
    int token;//表示当前令牌桶的令牌数
    int cps;//表示当前令牌桶的速率
    int burst;//当前令牌桶的容量
}tbf_t;

static tbf_t *tbf_libs[TBFMAX];
static int initd;

static void alarm_handler(int)
{
    int i = 0;
    alarm(1);
    for(;i < TBFMAX;i++)
    {
        if(tbf_libs[i] != NULL)
        {
            tbf_libs[i]->token += tbf_libs[i]->cps;
            if(tbf_libs[i]->token > tbf_libs[i]->burst)
                tbf_libs[i]->token = tbf_libs[i]->burst;
        }
    }
}

static void module_load(void)
{
    signal(SIGALRM, alarm_handler);
    alarm(1);
}
static int get_tbf_pos(void)
{
    int i = 0;
    for(;i < TBFMAX;i++)
    {
        if(tbf_libs[i] == NULL)
            return i;
    }
    return -1;
}

int tbf_init(int cps,int burst)
{
    int pos = 0;

    if(cps <= 0 || burst <= 0)
        return -1;

    if(!initd)
    {
        module_load();
        initd = 1;
    }
    pos = get_tbf_pos();
    if(pos < 0)
        return -2;
    
    tbf_libs[pos] = malloc(sizeof(tbf_t));
    if(tbf_libs[pos] == NULL)
        return -3;
    tbf_libs[pos]->cps = cps;
    tbf_libs[pos]->burst = burst;
    tbf_libs[pos]->token = 0;

    return pos;
}

int tbf_fetch_token(int td,int n)
{
    int fetch_token = 0;
    if(td < 0 || td >= TBFMAX || n <=0)
        return -1;
    if(tbf_libs[td] == NULL)
        return -2;
    while(tbf_libs[td]->token <= 0)
        pause();
    if(tbf_libs[td]->token >= n)
        fetch_token = n;
    else
        fetch_token = tbf_libs[td]->token;
    tbf_libs[td]->token -= fetch_token;

    return fetch_token;
}

int tbf_destroy(int td)
{
    if(td < 0 || td >= TBFMAX)
        return -1;
    if(tbf_libs[td] == NULL)
        return -2;
    free(tbf_libs[td]);
    tbf_libs[td] = NULL;

    return 0;
}











