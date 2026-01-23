#ifndef __QUEUE_H
#define __QUEUE_H


#define QUEUESIZE 6


typedef struct queue
{
	
	int queue_arr[QUEUESIZE];
	int queue_front;
	int queue_rear;
}QUEUE;


QUEUE *queue_create();

int  queue_en(QUEUE *q,int data);

void queue_display(QUEUE *q);

void queue_distroy(QUEUE *q);

int queue_de(QUEUE *q,int *save);

#endif
