#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

STACK *stack_create(void)
{
    STACK *s = NULL;//s指针指向开辟的栈式存储结构

    s = malloc(sizeof(STACK));//开辟栈式存储结构
    if(s == NULL)//判断开辟是否失败
        return NULL;//由于开辟失败,结束函数,并且返回NULL
    s->stack_ind = 0;//初始化栈顶元素

    return s;//返回开辟的栈式存储结构
}

int is_full(STACK *s)
{
    return !(s->stack_ind - STACKSIZE);
}

int is_empty(STACK *s)
{
    return !(s->stack_ind);
}

int stack_push(STACK *s, int data)
{
    if(is_full(s))//判断栈是否满了
        return -1;//由于栈满了,结束函数,并且返回-1
    s->stack_arr[s->stack_ind++] = data;//入栈
    return 0;
}

int stack_pop(STACK *s, int *save)
{
    if(is_empty(s))//判断栈是否空了
        return -1;//由于栈空了,结束函数,并且返回-1
    *save = s->stack_arr[--s->stack_ind];
    return 0;
}

void stack_display(STACK *s)
{
    int i = s->stack_ind;//使用i变量代替栈顶元素访问栈式存储空间

    if(is_empty(s))//判断栈是否空了
        return ;//由于栈空了,结束函数

    while(i--)
        printf("    %d\n", s->stack_arr[i]);
    printf("------------------------------\n\n");
}

void stack_destroy(STACK *s)
{
    if(s == NULL)//判断是否没有栈
        return ;
    free(s);//释放栈
}
