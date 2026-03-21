#ifndef __LIST_H
#define __LIST_H

#define NAMESIZE 20

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*op_print)(const void *data);
typedef int (*op_cmp)(const void *data,const void *key);


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

void list_display(LIST *head,op_print print);

void list_distroy(LIST *head);

struct list_node *list_find(LIST *head,const void *data,op_cmp cmp);

int list_delete(LIST *head,const void *data,op_cmp cmp);

int list_fetch(LIST *head,void *save,const void *data,op_cmp cmp);

#endif
