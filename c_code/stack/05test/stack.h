#ifndef __STACK_H
#define __STACK_H

#define STACKSIZE 32

typedef struct stack
{
    int stack_arr[STACKSIZE];//栈式的存储空间
    int stack_ind;//栈顶元素
}STACK;

STACK *stack_create(void);

int is_full(STACK *s);

int is_empty(STACK *s);

int stack_push(STACK *s, int data);

int stack_pop(STACK *s, int *save);

void stack_display(STACK *s);

void stack_destroy(STACK *s);

#endif

