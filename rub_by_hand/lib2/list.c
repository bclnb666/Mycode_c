#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


LIST *list_crreate(int size)
{
	LIST *head = NULL;
	head = malloc(sizeof(LIST));
	if(head == NULL)
		return NULL;
	head->size = size;
	head->h.prev = head->h.next = &head->h;

	return head;
}

void list_insert(LIST *head,const void *data,int mode)
{
	if(head == NULL)
		return;
	struct list_node *newnode = NULL;
	struct list_node *p = &head->h;

	newnode = malloc(head->size + sizeof(struct list_node));
	if(newnode == NULL)
		return;

	memcpy(newnode->data,data,head->size);

	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev; break;
		default : free(newnode); return;
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

}

void list_display(LIST *head,client_print print)
{
	struct list_node *cur = head->h.next;

	for(;cur != &head->h;cur = cur->next)
		print(cur->data);
}

void list_destroy(LIST *head)
{

	struct list_node *cur = head->h.next;

	for(;cur != &head->h;cur = head->h.next)
	{
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
	}
	free(head);
}

static struct list_node *_find(LIST *head,const void *data,client_cmp cmp)
{
	struct list_node *cur = head->h.next;
	for(;cur != &head->h;cur = cur->next)
	{
		if(cmp(cur->data,data))
			return cur;
	}
	return NULL;
}

void *list_find(LIST *head,const void *data,client_cmp cmp)
{
	struct list_node *cur = NULL;

	cur = _find(head,data,cmp);
	if(cur != NULL)
		return cur->data;
	else
		return NULL;
}

int list_delete(LIST *head,const void *data,client_cmp cmp)
{
	struct list_node *cur = NULL;
	cur = _find(head,data,cmp);
	if(cur == NULL)
		return -1;
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur);
	return 0;
}
