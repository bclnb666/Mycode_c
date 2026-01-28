#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

struct llist_node//数据节点的结构
{
    struct llist_node *prev;//前驱指针
    struct llist_node *next;//后继指针
    char data[0];//用来标记数据域的首地址
};

struct llist_head//头节点的结构
{
    int size;//用来存储用户指定数据域的大小
    struct llist_node head;//表示头节点
};

LLIST *llist_create(int size)
{
    struct llist_head *handler = NULL;//handler指针指向开辟的头节点

    handler = malloc(sizeof(struct llist_head));//开辟头节点的空间
    if(handler == NULL)//判断开辟头节点是否失败
        return NULL;//由于开辟头节点失败,结束函数,并且返回NULL

    handler->size = size;//存储用户指定数据域大小
    handler->head.prev = handler->head.next = &handler->head;//头节点的首尾指向自己

    return handler;//返回头节点的首地址
}

int llist_insert(LLIST *handler,const void *data, int mode)
{
    struct llist_head *h = handler;//转换指针
    struct llist_node *p = &h->head;//p指针代替头节点做操作
    struct llist_node *newnode = NULL;//newnode指针指向新开辟的数据节点

    newnode = malloc(sizeof(struct llist_node) + h->size);//开辟数据节点的空间
    if(newnode == NULL)//判断开辟数据节点的空间是否失败
        return -1;//由于开辟数据节点的空间失败,结束函数,并且返回-1

    memcpy(newnode->data, data, h->size);//拷贝数据

    switch(mode)
    {
        case HEADINSERT : break;
        case TAILINSERT : p = p->prev; break;
        default : free(newnode); return -2;
        //由于插入模式失败,释放新开辟的数据节点,结束函数,并且返回-2
    }
    newnode->next = p->next;
    newnode->prev = p;
    newnode->prev->next = newnode;
    newnode->next->prev = newnode;

    return 0;
}

void llist_display(LLIST *handler, llist_op op)
{
    struct llist_head *h = handler;//转换指针
    struct llist_node *cur = NULL;//cur指针指向每一个数据节点
    
    for(cur = h->head.next; cur != &h->head; cur = cur->next)
        op(cur->data);
}

void llist_destroy(LLIST *handler)
{   
    struct llist_head *h = handler;//转换指针
    struct llist_node *cur = h->head.next;//cur指针指向每一个数据节点
    
    while(cur != &h->head)
    {
        cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
        free(cur);
        cur = h->head.next;
    }
    free(h);
}

static struct llist_node *_find(LLIST *handler, const void *find_data, llist_cmp cmp)
{
    struct llist_head *h = handler;//转换指针
    struct llist_node *cur = NULL;//cur指针指向每一个数据节点

    for(cur = h->head.next; cur != &h->head; cur = cur->next)
        if(cmp(cur->data, find_data))//判断是否找到数据节点
            return cur;//返回找到的数据节点的地址
    return NULL;
}

void *llist_find(LLIST *handler, const void *find_data, llist_cmp cmp)
{
    struct llist_node *cur = NULL;

    cur = _find(handler, find_data, cmp);
    if(cur != NULL)
        return cur->data;
    else
        return NULL;
}

int llist_delete(LLIST *handler, const void *find_data, llist_cmp cmp)
{
    struct llist_node *cur = NULL;//cur指针指向要删除的节点

    cur = _find(handler, find_data, cmp);//查找要删除的节点
    if(cur == NULL)//判断是否没有找到
        return -1;//由于没有找到,结束函数,并且返回-1

    cur->next->prev = cur->prev;
    cur->prev->next = cur->next;
    free(cur);

    return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, llist_cmp cmp, void *save)
{
    struct llist_head *h = handler;//转换指针
    struct llist_node *cur = NULL;//cur指针指向要取出数据的节点

    cur = _find(handler, find_data, cmp);//查找要取出数据的节点
    if(cur == NULL)//判断是否没有找到
        return -1;//由于没有找到,结束函数,并且返回-1

    cur->next->prev = cur->prev;
    cur->prev->next = cur->next;
    memcpy(save, cur->data, h->size);//拷贝数据
    free(cur);//再释放数据节点

    return 0;
}

