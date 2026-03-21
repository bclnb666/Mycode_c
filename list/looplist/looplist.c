#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "looplist.h"


list *list_create(list *head)
{	
	head = malloc(sizeof(list));

	if(head == NULL){
		fprintf(stderr,"头节点创建失败！\n");
		return NULL;
	}
	head->next = head;
	return head;
}

int list_insert(list *head,const list *date,int mode)
{
	list *newnode = NULL;
	list *cur = head;

	newnode = malloc(sizeof(list));
	if(newnode == NULL){
		return -1;
	}
	
	memcpy(newnode,date,sizeof(list));

	switch(mode){
		case HEADINSERT : break;
		case TAILINSERT : while(cur->next != head) cur = cur->next; break;

		default : free(newnode); return -2;
	}
	newnode->next = cur->next;
	cur->next = newnode;

	return 1;
}

void list_display(list *head)
{
	list *cur = head->next;
	for(;cur != head;cur = cur->next){
		printf("%d %s %d\n",cur->id,cur->name,cur->math);
	}
}

static list *_find(list *head,const void *find_date)
{
	list *cur = head->next;
	for(;cur != head;cur = cur->next){
		if(cur->id == *(int *)find_date || !strcmp(cur->name,find_date)){
			//printf("找到了：%d %s %d\n",cur->id,cur->name,cur->math);
			return cur;
		}
	}
	return NULL;
}


list *list_find(list *head,const void *find_date)
{
	return _find(head,find_date);
}

int list_delete(list *head,const void *find_date)
{
	list *back = head;
	list *cur = _find(head,find_date);
	if(cur==NULL)
		return -1;
	while(back->next!=cur)
		back = back->next;
	back->next = cur->next;
	free(cur);
	return 0;
}

int list_change(list *head,const void *find_date)
{

	list *cur = _find(head,find_date);
	if(cur==NULL)
		return -1;
	cur->math = 9999;
	return 0;
}
