#include<stdio.h>
#include<string.h>
struct stu
{
	char name[20];
	int age;
	char sex[3];	
};
struct boy
{
	char name[20];
	int age;
	char sex[3];
};
int main()
{
	struct stu bcl={
		"bcl",22,"男"
	};
	struct boy b;

	printf("姓名:%s\n年龄:%d\n性别:%s\n",bcl.name,bcl.age,bcl.sex);

	memcpy(&b,&bcl,sizeof(b));
	printf("姓名:%s\n年龄:%d\n性别:%s\n",b.name,b.age,b.sex);
	printf("%ld\n",sizeof(bcl));
	printf("name = %ld\n",sizeof(bcl.name));
	printf("age = %ld\n",sizeof(bcl.age));
	printf("sex = %ld\n",sizeof(bcl.sex));
	return 0;
}
