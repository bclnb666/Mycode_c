#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>


LIST *list_creat(int size)
{
	LIST *head = NULL;
	head = malloc(sizeof(LIST));
	if(head == NULL){
		return NULL;
	}
	
	head->size = size;
	head->h.prev = &head->h;
	head->h.next = &head->h;

	return head;
}

int list_insert(LIST *head,const void *data,int mode)
{
	struct list_node *newnode = NULL;
	struct list_node *p = &head->h;
	newnode = malloc(sizeof(struct list_node));
	if(newnode == NULL)
		return -1;
	newnode->data = malloc(head->size);
	if(newnode->data == NULL){
		free(newnode);
		return -2;
	}

	memcpy(newnode->data,data,head->size);

	switch(mode){
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev;break;
		default : free(newnode->data);free(newnode);break;
	}

	newnode->next = p->next;
	newnode->prev = p;
    newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return 0;

}
void list_display(LIST *head,list_op op)
{
	struct list_node *cur = NULL;
	for(cur = head->h.next; cur != &head->h;cur = cur->next){
		op(cur->data);
	}
}

void list_destroy(LIST *head)
{
	struct list_node *cur = NULL;
	for(cur = head->h.next; cur != &head->h;cur = cur->next){
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur->data);
		free(cur);
		cur = head->h.next;
	}
	free(head);
}

static struct list_node *_find(LIST *head,const void *find_data,list_cmp cmp)
{
	struct list_node *cur = NULL;
	
	for(cur = head->h.next; cur != &head->h;cur = cur->next){
		if(cmp(cur->data,find_data))
			return cur;
	}
	return NULL;

}


struct list_node *list_find(LIST *head,const void *find_data,list_cmp cmp)
{
	
	struct list_node *cur = NULL;
	cur = _find(head,find_data,cmp);
	if(cur != NULL){
		return cur->data;
	}else{
		return NULL;
	}
}

int list_delete(LIST *head,const void *find_data,list_cmp cmp)
{
	
	struct list_node *cur = NULL;
	cur = _find(head,find_data,cmp);
	if(cur == NULL)
		return -1;
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur->data);
	free(cur);
	return 0;
}

int list_fetch(LIST *head,const void *find_data,list_cmp cmp,void *save)
{

	struct list_node *cur = NULL;
	cur = _find(head,find_data,cmp);
	if(cur == NULL)
		return -1;

	memcpy(save,cur->data,head->size);

	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur->data);
	free(cur);
	return 0;
}
