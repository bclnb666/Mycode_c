
#include <stdio.h>
#include "queue.h"


//int queue_arr[QUEUESIZE];
//int queue_front;
//int queue_rear;

int main()
{
	int arr[] = {
		11,22,33,44,55,66,77,88,99
	};
	int ret = 0;

	for(int i = 0;i<sizeof(arr)/sizeof(arr[0]);i++){
		ret = queue_en(arr[i]);
		if(ret != 0){
			printf("Queue Is Full!\n");
			break;
		}
	}
	queue_display();
	
	int save = 0;

	ret = queue_de(&save);
	if(ret != 0){
		printf("Queue Is Empty!\n");
	}else{
		printf("save = %d",save);
	}

	queue_display();

	queue_en(1024);

	queue_display();

	return 0;
}



