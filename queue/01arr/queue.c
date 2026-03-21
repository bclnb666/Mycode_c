#include <stdio.h>
#include "queue.h"

int queue_arr[QUEUESIZE];
int queue_front;
int queue_rear;

static int is_full()
{
	return ((queue_rear + 1) % QUEUESIZE) == queue_front;
}

static int is_empty()
{
	return queue_rear == queue_front;
}


int  queue_en(int data)
{
	if(is_full()){
		return -1;
	}
	queue_rear = (queue_rear + 1) % QUEUESIZE;
	queue_arr[queue_rear] = data;

	return 0;

}
int queue_de(int *save)
{
	if(is_empty()){
		return -1;
	}
	queue_front = (queue_front + 1) % QUEUESIZE;
	*save = queue_arr[queue_front];

	return 0;
}

void queue_display()
{

	if(is_empty()){
		return;
	}
	printf("\n-------------------------------\n");
	int i = (queue_front + 1) % QUEUESIZE;

	do{
		printf("%d ",queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}while(i != queue_rear);

	printf("%d ",queue_arr[i]);
	printf("\n-------------------------------\n\n");

}
