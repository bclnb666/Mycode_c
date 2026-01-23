#include <stdio.h>

void itob(int a);

int main()
{
	int a;
	scanf("%d",&a);
	itob(a);
	return 0;
}

void itob(int a)
{
	while(a){
		printf("%d",a%2);
		a/=2;
	}
	printf("\n");
}
