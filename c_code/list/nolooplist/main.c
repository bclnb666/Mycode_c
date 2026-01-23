#include<stdio.h>
#include"llist.h"



int main()
{
	LLIST *handler = NULL;
	LLIST data;

	handler = llist_creat();
	if(handler==NULL){
		fprintf(stderr,"llist_create() Is Failed!\n");
		return -1;
	}

	for(int i=0;i<5;i++){
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 100-i;
		data.next = NULL;
		llist_insert(handler,&data,HEADINSERT);
	}
	llist_diaplay(handler);
	int ret = llist_delete(handler,"stu1");
	if(ret!=1)
		printf("Delete is failed!\n");
	else
		printf("Delete it!\n");
	
	printf("---------------------------\n");
	llist_diaplay(handler);

	llist_destroy(handler);

	return 0;
}
