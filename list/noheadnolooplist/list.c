#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>

LIST *list_insert(LIST *l,const void *data,int mode)
{
	if(l == NULL){
		l = malloc(sizeof(LIST));
		if(l == NULL){
			return NULL;
		}
		memcpy(l,data,sizeof(LIST));
		return l;
	}

	LIST *newnode = NULL;
	newnode = malloc(sizeof(LIST));
	if(newnode == NULL){
		return l;
	}
	LIST *cur = l;
	
	memcpy(newnode,data,sizeof(LIST));

	switch(mode){
		case HEADINSERT : newnode->next = cur;l = newnode;break;
		case TAILINSERT : while(cur->next != NULL) cur = cur->next;cur->next = newnode; break;
		default : free(newnode); return l;
	}
	return l;
}

void list_display(LIST *l)
{
	LIST *cur = l;
	if(cur == NULL){
		return;
	}
	for(;cur->next != NULL;cur = cur->next){
		printf("%d %s %d\n",cur->id,cur->name,cur->math);
	}
}

static LIST *_find(LIST *l,const void *data)
{
	LIST *cur = l;
	for(;cur != NULL;cur = cur->next){
		if(cur->id == *(int *)data || !(strcmp(cur->name,data))){
			return cur;
		}
	}
	return NULL;
}

LIST *list_find(LIST *l,const void *data)
{
	return _find(l,data);
}

void list_distroy(LIST *l)
{
	if(l == NULL){
		return;
	}
	LIST *cur = l->next;
	for(;cur != NULL;cur = l->next){
		l->next = cur->next;
		free(cur);
	}
	free(l);
}

 int list_delete(LIST *l,const void *data)
{
	LIST *cur = _find(l,data);
	if(cur == NULL){
		return -1;
	}
	if(l==cur){
		l = l->next;
		free(cur);
		return 0;
	}else{
		LIST *back = NULL;
		for(back = l;back->next != cur;back = back->next);
		back->next = cur->next;
		free(cur);
		return 0;
	}
}

int list_fetch(LIST *l, const void *data, void *save)
{
	LIST *cur = _find(l,data);
	if(cur == NULL){
		return -1;
	}
	if(l==cur){
		l = l->next;
    	memcpy(save, cur, sizeof(LIST));
		free(cur);
		return 0;
	}else{
		LIST *back = NULL;
		for(back = l;back->next != cur;back = back->next);
		back->next = cur->next;
    	memcpy(save, cur, sizeof(LIST));
		free(cur);
		return 0;
	}

}
