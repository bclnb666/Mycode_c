#ifndef __LIST_H
#define __LIST_H

#define NAMESIZE 20
#define HEADINSERT 0
#define TAILINSERT 1


typedef struct list_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct list_node *next;
	struct list_node *prev;
}LIST;


LIST *list_create();

int list_insert(LIST *head,LIST *data,int mode);

void list_display(LIST *head);

void list_destroy(LIST *head);

LIST *list_find(LIST *head,const void *find_data);

int list_delete(LIST *head,const void *find_data);

int list_fetch(LIST *head,const void *find_data,void *save);


#endif
