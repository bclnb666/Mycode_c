#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <string.h>



int main()
{
	QUEUE *q = NULL;
	q = queue_create();
	if(q == NULL){
		fprintf(stderr,"queue_create() Is Failed!\n");
		return -1;
	}
	
	int arr[] = {
		11,22,33,44,55,66,77,88,99
	};
	int ret = 0;

	for(int i = 0;i < sizeof(arr) / sizeof(arr[0]);i++){
		ret = queue_en(q,arr[i]);
		if(ret != 0){
			printf("Queue Is Full!\n");
			break;
		}
	}
	queue_display(q);
	
	int save = 0;

	ret = queue_de(q,&save);
	if(ret != 0){
		printf("Queue Is Empty!\n");
	}else{
		printf("save = %d\n",save);
	}

	queue_display(q);

	queue_en(q,1024);

	queue_display(q);

	queue_distroy(q);
	return 0;
}



