#ifndef __LIST_H
#define __LIST_H

#define HEADINSERT 0
#define TAILINSERT 1
#define NAMESIZE 20

typedef struct list_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct list_node *next;
}LIST;

LIST *list_insert(LIST *l,const void *data,int mode);

void list_display(LIST *l);

LIST *list_find(LIST *l,const void *data);

void list_distroy(LIST *l);

int list_delete(LIST *l,const void *data);

int list_fetch(LIST *l, const void *data, void *save);

#endif
