#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdlib.h>

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)
{
	const struct stu *d = data;
	printf("%d %s %d %d\n",d->id,d->name,d->math,d->tel);
}


int id_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{
    const struct stu *p = data;//转换指针
    const int *k = key;//转换指针

    return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{
    const struct stu *p = data;//转换指针
    const int *k = key;//转换指针

    return !(p->tel - *k);
}

int name_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{
    const struct stu *p = data;//转换指针
    const char *k = key;//转换指针

    return !strcmp(p->name, k);
}


int main()
{
	LIST *head = NULL;
	struct list_node *find = NULL;
	int find_id = 101;
	char *find_name = "stu3";
	struct stu save;
	struct stu data;
	head = list_creat(sizeof(struct stu));
	int mode = HEADINSERT;
	if(head == NULL){
		fprintf(stderr,"创建头结点失败!\n");
		return -1;
	}

	for(int i = 1; i < 5;i++){
		data.id = i + 100;
		sprintf(data.name,"stu%d",i);
		data.math = i + 60;
		data.tel = 10000 + i;
		list_insert(head,&data,mode);
	}

	list_display(head,print);

	find = list_find(head,&find_id,id_cmp);
	if(find == NULL){
		printf("没找到！\n");
	}else{
		printf("找到了:");
		print(find);
	}

	int ret = list_delete(head,&find_id,id_cmp);
	if(ret != 0){
		printf("没找到删除失败！\n");
	}

	list_display(head,print);\
	

	int ret2 = list_fetch(head,find_name,name_cmp,&save);

	if(ret2 != 0){
		printf("没找到取出失败！\n");
	}
	printf("取出后：------------------------------\n");
   	

	list_display(head,print);


	printf("%ld\n",sizeof(LIST));

	list_destroy(head);

	return 0;
}
