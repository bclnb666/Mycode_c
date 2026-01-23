#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <string.h>

static int is_full(QUEUE *q)
{
	return 0;
}

static int is_empty(QUEUE *q)
{
	return llist_is_empty(q);
}

QUEUE *queue_create()
{
	return llist_create();
}


int  queue_en(QUEUE *q,int data)
{
	if(is_full(q)){
		return -1;
	}

	return llist_insert(q,&data,TAILINSERT);

}
int queue_de(QUEUE *q,int *save)
{
	if(is_empty(q)){
		return -1;
	}
	return llist_pop(q,save);
}

void queue_display(QUEUE *q)
{

	if(is_empty(q)){
		return;
	}
	llist_display(q);

}
void queue_distroy(QUEUE *q)
{
	if(is_empty(q)){
		return;
	}
	llist_destroy(q);
}
