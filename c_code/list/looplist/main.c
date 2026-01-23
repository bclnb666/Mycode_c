#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "looplist.h"

int main()
{
	list date;
	int mode = 1;
	int f_id = 1024;
	list *head = NULL;
	
	head = list_create(head);

	for(int i = 0;i < 5; i++){
		date.id = 1024 + i;
		sprintf(date.name,"stu%d",i);
		date.math = 100 - i;

		list_insert(head,&date,mode);
	}

	list_display(head);
	
	list *p = NULL;
	p = list_find(head,&f_id);
	if(p==NULL){
		printf("没找到！\n");
	}else{
		printf("找到了：%d %s %d\n",p->id,p->name,p->math);
	}

	int d = list_delete(head,"stu2");
	if(d!=0){
		printf("没找到，删除失败！\n");
	}


	list_display(head);

	int c = list_change(head,"stu1");
	if(c!=0){
		printf("没找到，更改失败！\n");
	}

	list_display(head);
	return 0;
}
