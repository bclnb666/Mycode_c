#ifndef __LIST_H
#define __LIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*list_op)(const void *);
typedef int (*list_cmp)(const void *,const void *);

struct list_node
{
	struct list_node *prev;
	struct list_node *next;
	char data[0];
};

typedef struct list_head
{
	int size;
	struct list_node h;
}LIST;


LIST *list_create(int size);

void list_insert(LIST *head,const void *data,int mode);

void list_display(LIST *head,list_op);

void *list_find(LIST *head,const void *data,list_cmp cmp);

int list_delete(LIST *head,const void *data,list_cmp cmp);

int list_fetch(LIST *head,const void *data,list_cmp cmp,void *save);

void list_distroy(LIST *head);


#endif
