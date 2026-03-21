#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST *llist_create(void)
{   
    LLIST *handler = NULL;//handler指针指向开辟的头节点
    
    handler = malloc(sizeof(LLIST));//开辟头节点
    if(handler == NULL)//判断辟开头节点是否失败
        return NULL;//由于开辟头节点失败,结束函数,并且返回NULL
    
    handler->prev = handler->next = handler;//让头节点的前驱指针和后继指针都指向自己
    
    return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{   
    LLIST *newnode = NULL;//newnode指针指向新开辟的节点
    LLIST *p = handler;//p指针代替handler指针做操作
    
    newnode = malloc(sizeof(LLIST));//开辟新的节点
    if(newnode == NULL)//判断新开辟的节点是否失败
        return -1;//由于新开辟的节点失败,结束函数,并且返回-1
    
    memcpy(newnode, data, sizeof(LLIST));//拷贝数据
    
    switch(mode)
    {
        case HEADINSERT : break;
        case TAILINSERT : p = p->prev; break;
        default : free(newnode); return -2;
        //由于插入模式失败,释放开辟的新节点,结束函数,并且返回-2
    }
    newnode->next = p->next;
    newnode->prev = p;
    newnode->next->prev = newnode;
    newnode->prev->next = newnode;
    
    return 0;
}

void llist_display(LLIST *handler)
{   
    LLIST *cur = NULL;
    
    for(cur = handler->next; cur != handler; cur = cur->next)
        printf("    %d\n", cur->id);
    printf("-----------------------------\n\n");
}

void llist_destroy(LLIST *handler)
{   
    LLIST *cur = NULL;//cur指针指向要释放的节点
    
    for(cur = handler->next; cur != handler; cur = handler->next)
    {
        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
        free(cur);//释放数据节点
    }
    free(cur);//释放头节点
}

static LLIST *_find(LLIST *handler, const void *find_data)
{   
    LLIST *cur = NULL;//cur指针指向找到的节点
    
    for(cur = handler->next; cur != handler; cur = cur->next)
        if(cur->id == *(int *)(find_data))
            return cur;
    return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
    return _find(handler, find_data);
}

int llist_delete(LLIST *handler, const void *find_data)
{
    LLIST *cur = NULL;//cur指针指向要删除的节点

    cur = _find(handler, find_data);//查找要删除的节点
    if(cur == NULL)//判断是否没有找到要删除的节点
        return -1;//由于没有找到要删除的节点,结束函数,并且返回-1

    cur->next->prev = cur->prev;
    cur->prev->next = cur->next;
    free(cur);

    return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
    LLIST *cur = NULL;//cur指针指向要取出的节点

    cur = _find(handler, find_data);//查找要取出的节点
    if(cur == NULL)//判断是否没有找到要取出的节点
        return -1;//由于没有找到要取出的节点,结束函数,并且返回-1

    cur->next->prev = cur->prev;
    cur->prev->next = cur->next;
    memcpy(save, cur, sizeof(LLIST));
    free(cur);

    return 0;
}

int llist_is_empty(LLIST *handler)
{
    return (handler->next == handler);
}

int llist_pop(LLIST *handler, void *save)
{
    LLIST *cur = handler->next;//

    if(llist_is_empty(handler))
        return -1;
    cur->next->prev = cur->prev;
	cur->prev->next = cur->next;

    memcpy(save, &cur->id, sizeof(cur->id));
    free(cur);

    return 0;
}


