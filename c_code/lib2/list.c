#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


LIST *list_create(int size)
{
	LIST *head = malloc(sizeof(LIST));
	if(head == NULL)
		return NULL;
	head->size = size;
	head->h.prev = head->h.next = &head->h;
	return head;
}

void list_insert(LIST *head,const void *data,int mode)
{
	struct list_node *newnode = NULL;
	struct list_node *p = &head->h;

	newnode = malloc(sizeof(struct list_node) + head->size);
	if(newnode == NULL)
		return;

	memcpy(newnode->data,data,head->size);

	switch(mode){
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev; break;
		default : free(newnode);return;
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

}

void list_display(LIST *head,list_op op)
{
	struct list_node *cur = head->h.next;
	for(;cur != &head->h;cur = cur->next)
		op(cur->data);
}

static struct list_node *_find(LIST *head,const void *data,list_cmp cmp)
{

	struct list_node *cur = head->h.next;
	for(;cur != &head->h;cur = cur->next)
		if(cmp(cur->data,data))
			return cur;
	return NULL;
}

void *list_find(LIST *head,const void *data,list_cmp cmp)
{
	struct list_node *cur = NULL;
	cur = _find(head,data,cmp);
	if(cur != NULL)
		return cur->data;
	else
		return NULL;
}

void list_distroy(LIST *head)
{
	struct list_node *cur = head->h.next;

	for(;cur != &head->h;){

		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
		cur = head->h.next;
	}
	free(head);
}

int list_delete(LIST *head,const void *data,list_cmp cmp)
{
	struct list_node *cur = NULL;
	cur = _find(head,data,cmp);
	if(cur != NULL){
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
		return 0;
	}
	else
		return -1;
}

int list_fetch(LIST *head,const void *data,list_cmp cmp,void *save)
{
	struct list_node *cur = NULL;
	cur = _find(head,data,cmp);
	if(cur != NULL){
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		memcpy(save,cur->data,head->size);
		free(cur);
		return 0;
	}
	else
		return -1;
}


