#include<stdio.h>

#define MAX(A,B) A>B?A:B

int main()
{
	int a,b;
	scanf("%d %d",&a,&b);

	printf("MAX = %d\n",MAX(a,b));


	return 0;
}
