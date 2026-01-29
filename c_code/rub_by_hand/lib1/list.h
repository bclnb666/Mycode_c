#ifndef __LIST_H
#define __LIST_H

#define NAMESIZE 20
#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*client_print)(const void *);

typedef int (*client_cmp)(const void *,const void *);


struct list_node
{
	void *data;
	struct list_node *prev;
	struct list_node *next;
};

typedef struct list_head
{
	int size;
	struct list_node h;
}LIST;

LIST *list_crreate(int size);

void list_insert(LIST *head,const void *data,int mode);

void list_display(LIST *head,client_print print);

void list_destroy(LIST *head);

void *list_find(LIST *head,const void *data,client_cmp cmp);

int list_delete(LIST *head,const void *data,client_cmp cmp);


#endif
