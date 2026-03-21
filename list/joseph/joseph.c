#include <stdio.h>
#include <stdlib.h>
#include "joseph.h"

JOSEPH *joseph_insert(int p_n)
{   
    JOSEPH *j = NULL;//j指针指向约瑟夫环的第一个节点
    JOSEPH *p = NULL;//p指针代替j指针做操作
    JOSEPH *newnode = NULL;//newnode指针指向新创建的节点
    int i = 1;//循环变量
    
    j = malloc(sizeof(JOSEPH));
    j->id = i;
    j->next = j;
    p = j;//接下来用p指针代替j指针做操作
    
    for(i = 2; i <= p_n; i++)
    {
        newnode = malloc(sizeof(JOSEPH));
        newnode->id = i;
        newnode->next = p->next;
        p->next = newnode;
        p = newnode;
    }
    
    return j;
}

void joseph_display(JOSEPH *j)
{
    JOSEPH *cur = NULL;

    for(cur = j; cur->next != j; cur = cur->next)
        printf("%d ", cur->id);
    printf("%d\n", cur->id);
}

int joseph_play(JOSEPH *j, int p_n, int k_n)
{
    JOSEPH *back = NULL;//back指针指向要删除节点的前一个位置
    JOSEPH *cur = NULL;//cur指针指向要删除的节点
    int alive = 0;//存储存活下来的人的ID
    int i = 0;//循环变量

    for(back = j; back->next != j; back = back->next);//循环找到最后一个节点

    while(--p_n)
    {
        for(i = 0; i < k_n - 1; i++)
            back = back->next;
        cur = back->next;//cur指向要删除的节点
        back->next = cur->next;
        printf("%d ", cur->id);
        free(cur);
    }
    alive = back->id;
    free(back);

    return alive;
}
