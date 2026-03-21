#include<stdio.h>
#include<string.h>
typedef struct student
{
	char name[20];
	int age;
	char sex[3];	
}stu;

void gua(stu *p)
{

	p[0].age = 999;
}

int main()
{
	stu arr[2];
	for(int i=0;i<2;i++){
		scanf("%s",arr[i].name);
		scanf("%d",&arr[i].age);
		scanf("%s",arr[i].sex);
	}
	for(int i=0;i<2;i++){
		printf("姓名:%s\n年龄:%d\n性别:%s\n",arr[i].name,arr[i].age,arr[i].sex);
	}

	gua(arr);

	for(int i=0;i<2;i++){
		printf("姓名:%s\n年龄:%d\n性别:%s\n",arr[i].name,arr[i].age,arr[i].sex);
	}

	return 0;
}
