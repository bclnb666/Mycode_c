#ifndef __LIST_H
#define __LIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void (*list_op)(const void *);
typedef int (*list_cmp)(const void *,const void *);


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

LIST *list_creat(int size);

int list_insert(LIST *head,const void *data,int mode);

void list_display(LIST *head,list_op op);

void list_destroy(LIST *head);



struct list_node *list_find(LIST *head,const void *find_data,list_cmp cmp);


int list_delete(LIST *head,const void *find_data,list_cmp cmp);
	
int list_fetch(LIST *head,const void *find_data,list_cmp cmp,void *save);

#endif
