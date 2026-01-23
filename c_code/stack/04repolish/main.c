#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <string.h>

int  repolish(const char *postfix,int *result)
{
	int len = strlen(postfix);
	int i = 0;
	int num = 0;
	int num1 = 0,num2 = 0;
	STACK *num_stack = NULL;
	num_stack = stack_create();

	if(num_stack == NULL){
		fprintf(stderr,"num_stack:stack_create() Is Failed\n");
		return -1;
	}

	while(i < len){
		if(postfix[i]== ' '){
			i++;
			continue;
		}
		if(postfix[i] >= '0' && postfix[i] <= '9'){
			num = 0;
			while(i < len && postfix[i] >= '0' && postfix[i] <= '9'){
				num = num * 10 + (postfix[i] - '0');
				i++;
			}
			stack_push(num_stack,num);
		}else if(strchr("+-*/",postfix[i])){
				if(stack_pop(num_stack,&num2) == -1){
				printf("表达式有问题！\n");
				stack_destroy(num_stack);
				return -2;
			}
			if(stack_pop(num_stack,&num1) == -1){
			printf("表达式有问题！\n");
			stack_destroy(num_stack);
			return -3;
			}
			switch(postfix[i]){
				case '+' : stack_push(num_stack,num1 + num2);break;
				case '-' : stack_push(num_stack,num1 - num2);break;
				case '*' : stack_push(num_stack,num1 * num2);break;
				case '/' : if(num2 == 0){
							 printf("表达式中除数为零！\n");
							 stack_destroy(num_stack);
							 return -4;
						 }
						 stack_push(num_stack,num1 / num2);break;
				default : break;
			}
			i++;
		}else{
			stack_destroy(num_stack);
			return -7;
		}
	}

	if(stack_pop(num_stack,result) == -1){
		printf("表达式有问题！\n");
		stack_destroy(num_stack);
		return -5;
	}
	
	if(!is_empty(num_stack)){
		printf("表达式有问题，（操作数过多！）\n");
		stack_destroy(num_stack);
		return -6;
	}
	stack_destroy(num_stack);
	return 0;
}
int get_priority(char op)
{
	switch(op){
		case '(' :return 0;
		case '+' :
		case '-' :return 1;
		case '*' :
		case '/' :return 2;
		default :return -1;
	}
}


int infix_to_postfix(const char *infix,char *postfix)
{
	STACK *op_stack = NULL;
	int i = 0,j = 0;
	int len = strlen(infix);
	int op = 0;
	int cur_op = 0;
	int top_op = 0;
	int ret = 0;
	int result = 0;
	op_stack = stack_create();
	if(op_stack == NULL){
		fprintf(stderr,"op_stack:stack_create() Is Failed\n");
		return -1;
	}
	while(i < len){
		if(infix[i]== ' '){
			i++;
			continue;
		}
		if(infix[i] >= '0' && infix[i] <= '9'){
			while(i < len && infix[i] >= '0'&& infix[i] <= '9'){
				postfix[j++] = infix[i++];
			}
			postfix[j++] = ' ';
		}else if(infix[i] == '('){
			stack_push(op_stack,infix[i]);
			i++;
		}else if(infix[i] == ')'){
			while(!is_empty(op_stack)){
				stack_pop(op_stack,&op);
				if(op == '('){
					break;
				}
				postfix[j++] = op;
				postfix[j++] = ' ';
			}
			i++;
		}else if(strchr("+-*/",infix[i])){
			cur_op = infix[i];
			while(!is_empty(op_stack)){
				stack_pop(op_stack,&top_op);
				if(get_priority(top_op) < get_priority(cur_op)){
					stack_push(op_stack,top_op);
					break;
				}
				postfix[j++] = top_op;
				postfix[j++] = ' ';
			}
			stack_push(op_stack,cur_op);
			i++;
		}else{
			printf("出现错误的字符 ：%c\n",infix[i]);
			stack_destroy(op_stack);
			return -2;
		}		
	}
	while(!is_empty(op_stack)){
		stack_pop(op_stack,&op);
		postfix[j++] = op;
		postfix[j++] = ' ';
	}
	postfix[j-1] = '\0';
	stack_destroy(op_stack);
	return 0;
}

int main()
{

	char infix[32] = {0};
	char postfix[32] = {0};
	int ret = 0;
	int result = 0;

	printf("请输入中缀表达式:");

	fgets(infix,sizeof(infix),stdin);

	infix[strcspn(infix,"\n")] = '\0';

	ret = infix_to_postfix(infix,postfix);
	if(ret != 0){
		fprintf(stderr,"infix_to_postfix() Is Failed\n");
		return -1;
	}
	
	//printf("infix : %s\n",infix);
	printf("转换后的后缀表达式: %s\n",postfix);

	ret = repolish(postfix,&result);
	if(ret != 0){
		printf("后缀表达式有问题！\n");
	}else{
		printf("result = %d\n",result);
	}
		
	return 0;
}
