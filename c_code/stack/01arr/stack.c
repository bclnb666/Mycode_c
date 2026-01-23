#include <stdio.h>
#include "stack.h"


int stack_arr[STACKSIZE];
int stack_top;


static int is_full()
{
	return !(stack_top - STACKSIZE);
}

static int is_empty()
{
	return !(stack_top);
}

int stack_push(int data)
{
	if(is_full()){
		return -1;
	}
	stack_arr[stack_top++] = data;

	return 0;
}

int stack_pop(int *save)
{
	
	if(is_empty()){
		return -1;
	}
	*save = stack_arr[--stack_top];
	return 0;
}

void stack_display()
{
	if(is_empty()){
		return;
	}
	for(int i = stack_top-1;i >= 0; i--){
		printf("  %d\n",stack_arr[i]);
	}
	printf("----------------------------\n");


	
}

