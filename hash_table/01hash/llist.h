#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*llist_op)(const void *);//定义了函数指针类型
typedef int (*llist_cmp)(const void *, const void *);//定义了函数指针类型
typedef void LLIST;

LLIST *llist_create(int );

int llist_insert(LLIST *, const void *, int );

void llist_display(LLIST *, llist_op);

void llist_destroy(LLIST *);

void *llist_find(LLIST *, const void *, llist_cmp);

int llist_delete(LLIST *, const void *, llist_cmp);

int llist_fetch(LLIST *, const void *, llist_cmp, void *);

#endif
