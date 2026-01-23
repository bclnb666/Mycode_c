#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>

int main()
{
	LIST *head = NULL;
	LIST data;
	LIST save;
	int mode = TAILINSERT;
	LIST *find = NULL;
	char *find_name = "stu1";
	int find_id = 100;
	head = list_create();
	if(head == NULL){
		fprintf(stderr,"创建头结点失败!\n");
		return -1;
	}

	for(int i = 0;i < 5;i++){
		data.id = i + 100;
		sprintf(data.name,"stu%d",i);
		data.math = 100 - i;
		data.prev = data.next = NULL;

		list_insert(head,&data,mode);
	}
	
	list_display(head);
	
	find = list_find(head,find_name);
	if(find == NULL){
		printf("没找到！\n");
	}
	printf("找到了：%d %s %d\n",find->id,find->name,find->math);

	int d = list_delete(head,find_name);
	if(d != 0){
		printf("没找到，删除失败！\n");
	}
	printf("删除后：--------------------------\n");
	list_display(head);
	
	int ret  = list_fetch(head,&find_id,&save);
	if(ret != 0){
		printf("没找到，取出失败！\n");
	}
	printf("取出的数据：%d %s %d\n",save.id,save.name,save.math);
	printf("取出后：--------------------------\n");
	list_display(head);

	list_destroy(head);



	return 0;
}
