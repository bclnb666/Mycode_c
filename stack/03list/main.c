#include <stdio.h>
#include "stack.h"
#include <stdlib.h>
#include <string.h>


int main()
{
	STACK *s = NULL;

	s = stack_create();
	if(s == NULL){
		fprintf(stderr,"stack_create() Is Failed!\n");
		return -1;
	}

	int data[] = {11,22,33,44,55,66,77,88,99};

	int ret = 0;
	
	int save = 0;

	for(int i = 0;i < sizeof(data)/sizeof(data[0]);i++){
		ret = stack_push(s,data[i]);
		if(ret != 0){
			printf("Stack Is Full!\n");
			break;
		}
	}

	stack_display(s);

	//stack_destroy(s);

	ret = stack_pop(s,&save);
	if(ret != 0){
		printf("Stack Is Empty!\n");
	}else{
		printf("save = %d\n",save);
	}

	stack_display(s);

	stack_push(s,8888);

	stack_display(s);

	stack_destroy(s);
	return 0;
}
