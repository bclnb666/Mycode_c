#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct cookbook
{
	int id;
	char style_cook[STYLESIZE];
	char degree_dif[STYLESIZE];
	int time;
	int math;
};

void print(const void *data)
{
	const struct stu *p = data;
	printf("%d %s %d\n",p->id,p->name,p->math);
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
	struct stu insert;
	int find_id = 1;
	char *find_name = "bcl3";
	struct list_node *find = NULL;
	struct stu save;

	head = list_create(sizeof(struct stu));
	if(head == NULL){
		printf("list_create() Is Failed!\n");
		return 0;
	}

	for(int i = 0;i < 4;i++){
		insert.id = i;
		sprintf(insert.name,"bcl%d",i);
		insert.math = 100 - i;
		list_insert(head,&insert,HEADINSERT);
	}

	list_display(head,print);

	find = list_find(head,find_name,name_cmp);
	if(find == NULL){
		printf("No Find It!\n");
	}else{
		printf("Find It : ");
		print(find->data);
	}

	int ret = list_delete(head,find_name,name_cmp);
	if(ret != 0){
		printf("list_delete() Is Failed No Find It!\n");
	}else{
		printf("Delete It Success!\n");
		list_display(head,print);
	}


	ret = list_fetch(head,&save,&find_id,id_cmp);
	if(ret != 0){
		printf("list_fetch() Is Failed No Find It!\n");
	}else{
		printf("Fetch It Success :");
		print(&save);
		list_display(head,print);
	}


	return 0;
}

