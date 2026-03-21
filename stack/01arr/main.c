#include <stdio.h>
#include "stack.h"


extern int stack_arr[STACKSIZE];
extern int stack_top;

int main()
{
	int data[] = {11,22,33,44,55,66,77,88,99};

	int ret = 0;
	
	int save = 0;

	for(int i = 0;i < sizeof(data)/sizeof(data[0]);i++){

		ret = stack_push(data[i]);
		if(ret != 0){
			printf("Stack Is Full!\n");
			break;
		}
	}

	stack_display();


	ret = stack_pop(&save);
	if(ret != 0){
		printf("Stack Is Empty!\n");
	}else{
		printf("save = %d\n",save);
	}

	stack_display();

	stack_push(8888);

	stack_display();
	return 0;
}
