#ifndef __LLIST_H
#define __LLIST_H

#define NAMESIZE 20

enum E{HEADINSERT,TAILINSERT};


typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node *next;

}LLIST;

LLIST *llist_creat();

int llist_insert(LLIST *handler,const void *data,int mode);

void llist_diaplay(LLIST *handler);

void llist_destroy(LLIST *handler);

LLIST *llist_find(LLIST *handler,const void *data);

int llist_delete(LLIST *handler,const void *data);


int llist_fetch(LLIST *handler, const void *data, void *save);

#endif
