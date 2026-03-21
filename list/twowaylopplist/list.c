#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>


LIST *list_create()
{
	LIST *head = NULL;
	head = malloc(sizeof(LIST));
	if(head == NULL){
		return NULL;
	}
	head->prev = head->next = head;
	return head;
}
int list_insert(LIST *head,LIST *data,int mode)
{
	LIST *p = head;
	LIST *newnode = NULL;
	newnode = malloc(sizeof(LIST));
	if(newnode == NULL){
		return -1;
	}

	memcpy(newnode,data,sizeof(LIST));

	switch(mode){
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev;break;
		default : free(newnode); break; 
	}
	
	newnode->next = p->next;
	newnode->prev = p;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return 0;
}
void list_display(LIST *head)
{
	LIST *p = NULL;
	for(p = head->next; p != head; p = p->next){
		printf("%d %s %d\n",p->id,p->name,p->math);
	}
}
void list_destroy(LIST *head)
{
	LIST *p = NULL;
	for(p = head->next; p != head; p = head->next){
		p->prev->next = p->next;
		p->next->prev = p->prev;
		free(p);
	}
	free(p);
}

static LIST *_find(LIST *head,const void *find_data)
{
	LIST *p = NULL;
	for(p = head->next;p != head; p = p->next){
		if(p->id == *(int *)find_data || !strcmp(p->name,find_data)){
			return p;
		}
	}
	return NULL;
}


LIST *list_find(LIST *head,const void *find_data)
{
	return _find(head,find_data);
}

int list_delete(LIST *head,const void *find_data)
{
	LIST *p = NULL;
	p = _find(head,find_data);
	if(p == NULL)
		return -1;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p);
	return 0;
}

int list_fetch(LIST *head,const void *find_data,void *save)
{
	LIST *cur = NULL;
	cur = _find(head,find_data);
	if(save == NULL)
		return -1;
	
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save,cur,sizeof(LIST));
	free(cur);

	return 0;

}
