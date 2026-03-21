#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
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
	const int * k = key;

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
	struct stu data,save;
	int size = sizeof(struct stu);
	int mode = HEADINSERT;
	int find_id = 100;
	struct stu *find = NULL;
	char *delete_name = "Ammon3";

	
	head = list_create(size);
	if(head == NULL){
		fprintf(stderr,"list_create() Is Failed!\n");
		return 0;
	}
	for(int i = 0; i < 5;i++){
		data.id = i + 100;
		sprintf(data.name,"Ammon%d",i);
		data.math = 444 + i;
		list_insert(head,&data,mode);
	}

	list_display(head,print);
	
	find = list_find(head,&find_id,id_cmp);
	if(find == NULL){
		printf("Not Find!\n");
	}else{
		printf("Find It:");
		print(find);
	}

	int ret = list_delete(head,delete_name,name_cmp);
	if(ret != 0){
		printf("list_delete() Is Failed!\n");
	}else{
		printf("Delete Success!\n");
		list_display(head,print);
	}

	ret = list_fetch(head,&find_id,id_cmp,&save);
	if(ret != 0){
		printf("list_fetch() Is Failed!\n");
	}else{
		printf("Fetch Success :");
		print(&save);
		list_display(head,print);
	}


	list_distroy(head);

	return 0;
}
