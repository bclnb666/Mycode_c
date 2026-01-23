#include <stdio.h>
#include "queue.h"
#include <stdlib.h>
#include <string.h>

static int is_full(QUEUE *q)
{
	return ((q->queue_rear + 1) % QUEUESIZE) == (q->queue_front);
}

static int is_empty(QUEUE *q)
{
	return q->queue_rear == q->queue_front;
}

QUEUE *queue_create()
{
	QUEUE *q = NULL;
	q = malloc(sizeof(QUEUE));
	if(q == NULL){
		return NULL;
	}

	q->queue_front = q->queue_rear = 0;

	return q;
}


int  queue_en(QUEUE *q,int data)
{
	if(is_full(q)){
		return -1;
	}
	q->queue_rear = (q->queue_rear + 1) % QUEUESIZE;
	q->queue_arr[q->queue_rear] = data;

	return 0;

}
int queue_de(QUEUE *q,int *save)
{
	if(is_empty(q)){
		return -1;
	}
	q->queue_front = (q->queue_front + 1) % QUEUESIZE;
	*save = q->queue_arr[q->queue_front];

	return 0;
}

void queue_display(QUEUE *q)
{

	if(is_empty(q)){
		return;
	}
	printf("-------------------------------\n");
	int i = (q->queue_front + 1) % QUEUESIZE;

	do{
		printf("%d ",q->queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}while(i != q->queue_rear);

	printf("%d ",q->queue_arr[i]);
	printf("\n-------------------------------\n\n");

}
void queue_distroy(QUEUE *q)
{
	if(is_empty(q)){
		return;
	}
	free(q);
}
