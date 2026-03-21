#include <stdio.h>

int max(int a,int b);

int main()
{
	int a,b;
	scanf("%d %d",&a,&b);
	
	int better = max(a,b);

	printf("%d\n",better);

	return 0;
}

int max(int a,int b)
{
	return a<b?b:a;
}
