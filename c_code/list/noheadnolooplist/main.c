#include <stdio.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>



int main()
{
	LIST *l = NULL;
	LIST data;
	int mode = HEADINSERT;
	int find_id = 102;
	char *find_name = "stu3";
	LIST save;

	for(int i = 0;i < 5;i++){
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 88 - i;
		data.next = NULL;
		l = list_insert(l,&data,mode);
	}

	list_display(l);
	
	LIST *p = list_find(l,&find_id);
	if(p == NULL){
		printf("没找到!\n");
	}else{
		printf("找到了：%d %s %d\n",p->id,p->name,p->math);
	}

	int ret = list_delete(l,find_name);
	if(ret != 0){
		printf("没找到删除失败!\n");
	}else{
		printf("删除后：\n");
	}

	list_display(l);
	
	ret = list_fetch(l,"stu1",&save);

	if(ret != 0){
		printf("没找到取出失败!\n");
	}else{
		printf("save : %d %s %d\n",save.id,save.name,save.math);
	}

	list_display(l);
	list_distroy(l);
	return 0;
}
