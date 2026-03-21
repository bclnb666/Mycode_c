#ifndef __STACK_H
#define __STACK_H

#define STACKSIZE 6


typedef struct stack
{
	int stack_arr[STACKSIZE];
	int stack_top;
}STACK;

STACK *stack_create();


int stack_push(STACK *s,int data);

int stack_pop(STACK *s,int *save);

void stack_display(STACK *s);

void stack_destroy(STACK *s);

#endif
