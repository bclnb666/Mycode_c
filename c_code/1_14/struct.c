#include<stdio.h>
#include<string.h>
struct stu
{
	char name[20];
	int age;
	char sex[3];	
};
int main()
{
	struct stu bcl;
	strcpy(bcl.name,"bcl");
   	bcl.age = 22;	
	strcpy(bcl.sex,"男");

	printf("姓名:%s\n年龄:%d\n性别:%s\n",bcl.name,bcl.age,bcl.sex);
	return 0;
}
