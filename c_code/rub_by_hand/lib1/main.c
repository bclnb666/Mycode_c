#include <stdio.h>
#include <string.h>
#include "list.h"

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
};

void print(const void *data)
{
	const struct stu *d = data;
	printf("%d %s %d\n",d->id,d->name,d->math);
}

int id_cmp(const void *data,const void *key)
{
	const struct stu *p = data;
	const int *k = key;

	return !(p->id - *k);
}

int name_cmp(const void *data,const void *key)
{
	const struct stu *p = data;
	const char *k = key;

	return !strcmp(p->name,k);
}

int math_cmp(const void *data,const void *key)
{
	const struct stu *p = data;
	const int *k = key;

	return !(p->math - *k);
}

int main()
{
	LIST *head = NULL;
	struct stu data;
	int find_id = 3;
	struct stu *find = NULL;

	head = list_crreate(sizeof(struct stu));
   	if(head == NULL)
	{
		fprintf(stderr,"list_crreate() Is Failed!\n");
		return -1;
	}

	
	for(int i = 0;i < 5;i++)
	{
		data.id = i;
		sprintf(data.name,"Ammon%d",i);
		data.math = 99 - i;
		list_insert(head,&data,HEADINSERT);
	}

	list_display(head,print);

	find = list_find(head,&find_id,id_cmp);
	if(find == NULL)
	{
		printf("list_find() Is Failed!\n");
	}else
	{
		printf("Find It :");
		print(find);
	}
	int ret = list_delete(head,"Ammon2",name_cmp);
	if(ret != 0)
	{
		printf("list_delete() Is Failed!\n");
	}else
	{
		printf("Delete It Success!\n");
		list_display(head,print);
	}
	list_destroy(head);


	return 0;
}
