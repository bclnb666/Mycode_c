#ifndef __LOOPLIST_H
#define __LOOPLIST_H

#define NAMEMAX 20

enum E{
	HEADINSERT,
	TAILINSERT
};

typedef struct list_node{

	int id;
	char name[NAMEMAX];
	int math;
	struct list_node *next;

}list;


list *list_create(list *head);

int list_insert(list *head,const list *date,int mode);

void list_display(list *head);

list *list_find(list *head,const void *find_date);

int list_delete(list *head,const void *find_date);


int list_change(list *head,const void *find_date);

#endif
