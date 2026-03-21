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
	STACK *s = NULL;
	s = malloc(sizeof(STACK));
	if(s == NULL){
		return NULL;
	}
	s->stack_top = 0;

	return s;
}


void stack_destroy(STACK *s)
{
	if(s == NULL){
		return;
	}
	free(s);
}

static int is_full(STACK *s)
{
	return !(s->stack_top - STACKSIZE);
}

int is_empty(STACK *s)
{
	return !(s->stack_top);
}

int stack_push(STACK *s,int data)
{
	if(is_full(s)){
		return -1;
	}
	s->stack_arr[s->stack_top++] = data;
	return 0;
}

int stack_pop(STACK *s,int *save)
{
	
	if(is_empty(s)){
		return -1;
	}
	*save = s->stack_arr[--s->stack_top];
	return 0;
}

void stack_display(STACK *s)
{
	if(is_empty(s)){
		return;
	}
	for(int i = s->stack_top-1;i >= 0; i--){
		printf("  %d\n",s->stack_arr[i]);
	}
	printf("----------------------------\n");
}

