#include <stdio.h>

int main()
{
	int num;
	scanf("%d",&num);
	while(num){
		int a =num%10;
		
		printf("%d",a);

		num/=10;
	}
	printf("\n");
	return 0;
}
