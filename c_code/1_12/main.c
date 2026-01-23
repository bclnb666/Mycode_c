#include<stdio.h>
#include"add.h"

extern int max();

int a = 0, b = 0;

int main()
{
	int sum = 0;
	
	scanf("%d %d",&a,&b);
	sum = add(a,b);

	printf("sum = %d\n",sum);

	printf("max = %d\n",max());

	return 0;
}
