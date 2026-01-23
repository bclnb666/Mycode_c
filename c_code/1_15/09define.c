#include<stdio.h>

#define SWAP(A,B) A+=B;B=A-B;A-=B;

int main()
{
	int a,b;
	scanf("%d %d",&a,&b);

	printf("SWAP前 a = %d,b = %d\n",a,b);

	SWAP(a,b);

	printf("SWAP后 a = %d,b = %d\n",a,b);


	return 0;
}
