#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <string.h>
/*
static void init_stack(STACK *s)
{
	memset(s->stack_arr,0,sizeof());
	s->stack_top = 0;
}
*/

STACK *stack_create()
{
	return llist_create();
}


void stack_destroy(STACK *s)
{
	if(s == NULL)
		return;
	llist_destroy(s);
}

static int is_full(STACK *s)
{
	return 0;
}

static int is_empty(STACK *s)
{
	return llist_is_empty(s);
}

int stack_push(STACK *s,int data)
{
	if(is_full(s)){
		return -1;
	}
	llist_insert(s,&data,HEADINSERT);
	return 0;
}

int stack_pop(STACK *s,int *save)
{
	
	if(is_empty(s)){
		return -1;
	}
	llist_pop(s,save);
	return 0;
}

void stack_display(STACK *s)
{
	if(is_empty(s)){
		return;
	}
	llist_display(s);
}

