#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"llist.h"


LLIST *llist_creat()
{
	LLIST *handler = NULL;
	handler = malloc(sizeof(LLIST));
	//判断handler是否开辟成功
	if(handler == NULL)
		return NULL;
	
	handler->next = NULL;

	return handler;

}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	LLIST *p = handler;
	LLIST *newnode = NULL;
	newnode = malloc(sizeof(LLIST));
	if(newnode==NULL)
		return -1;

	memcpy(newnode,data,sizeof(LLIST));

	switch(mode){
		case HEADINSERT : break;
		case TAILINSERT : while(p->next != NULL){p = p->next;} break;
		default : free(newnode); return -2;
	}
	newnode->next = p->next;
	p->next = newnode;
	
	return 0;

}

void llist_diaplay(LLIST *handler)
{
	LLIST *cur = handler->next;
	for(;cur!= NULL;cur = cur->next){
		printf("%d %s %d\n",cur->id,cur->name,cur->math);
	}
}

void llist_destroy(LLIST *handler){
	LLIST *cur = NULL;
	LLIST *back = handler;
	for(cur = handler->next;cur != NULL;cur = back->next){
		back->next = cur->next;
		free(cur);
	}
	free(back);
}

static LLIST *_find(LLIST *handler,const void *data){
	LLIST *cur = handler;
	for(;cur!=NULL;cur=cur->next){
		if(cur->id==*(int*)data||!strcmp(cur->name,data))
			return cur;
	}
	return NULL;
}

LLIST *llist_find(LLIST *handler,const void *data){
	return _find(handler,data);
}

int llist_delete(LLIST *handler,const void *data)
{
	LLIST *cur = NULL;
	LLIST *back = handler;

	cur = _find(handler,data);
	if(cur==NULL)
		return -1;


	while(back->next!=cur){
		back = back->next;
	}
	back->next = cur->next;
	free(cur);
	return 1;
}

int llist_fetch(LLIST *handler, const void *data, void *save)
{
    LLIST *back = handler;
    LLIST *cur = NULL;

    cur = _find(handler,data);
    if(cur == NULL)
        return -1;

    while(back->next != cur)
        back = back->next;

    back->next = cur->next;
    memcpy(save, cur, sizeof(LLIST));
    free(cur);

    return 1;
}
