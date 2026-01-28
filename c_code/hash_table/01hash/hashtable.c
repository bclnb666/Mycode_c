#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"
#include "hashtable.h"

struct htable_head
{

	int size;//单个数据节点的数据域大小
	int capacity;//哈希表的容量（数组的长度）
	LLIST **bucket;//桶数组指针（每个元素是一个链表（LLIST*）的头结点
};


HTABLE *htable_create(int size,int capacity)
{
	if(size <= 0)
		return NULL;
	if(capacity <= 0)
		capacity = HTABLE_DEFAULT_CAPACITY;

	struct htable_head *handler = NULL;
	handler = malloc(sizeof(struct htable_head));
	if(handler == NULL)
		return NULL;
	handler->size = size;
	handler->capacity = capacity;
	handler->bucket = malloc(sizeof(LLIST *) * capacity);
	if(handler->bucket == NULL)
	{
		free(handler);
		return NULL;
	}
	for(int i = 0;i < capacity;i++)
	{
		handler->bucket[i] = llist_create(size);
		if(handler->bucket[i] == NULL)
		{
			for(int j = 0;j < i;j++)
			{
				llist_destroy(handler->bucket[j]);
			}
			free(handler->bucket);
			free(handler);
			return NULL;
		}
	}
	return handler;
}

int htable_insert(HTABLE *handler,const void *data,htable_hash hash,htable_cmp cmp,int mode)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	void *find_data = NULL;

	if(h == NULL || data ==NULL || hash == NULL || cmp == NULL)
		return -1;
	
	idx = hash(data,h->capacity);

	switch(mode)
	{
		case HTABLE_INSERT_REPLACE : 
			llist_delete(h->bucket[idx],data,cmp);//在对应桶中删除对应的数据
			break;
		case HTABLE_INSERT_NOSAME :
			find_data = llist_find(h->bucket[idx],data,cmp);
			if(find_data != NULL)
				return -2;
		case HTABLE_INSERT_SAME :break;
		default : return -3;
	}
	
	return llist_insert(h->bucket[idx],data,TAILINSERT);


}

void htable_display(HTABLE *handler,htable_op op)
{
	struct htable_head *h = handler;
	if(h == NULL || op == NULL)
		return;
	for(int i = 0;i < h->capacity;i++)
	{
		printf("BUCKET[%d]: ",i);
		llist_display(h->bucket[i],op);
		printf("\n");
	}
}

void htable_destroy(HTABLE *handler)
{
	struct htable_head *h = handler;
	if(h == NULL)
		return;
	for(int i = 0;i < h->capacity; i++)
	{
		llist_destroy(h->bucket[i]);
	}
	free(h->bucket);
	free(h);
}
