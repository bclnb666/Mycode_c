#include<stdio.h>

//#define ADD

int main()
{

	int a = 13,b = 7;


#ifndef ADD
	printf("%d - %d = %d\n",a,b,a - b);
#else
	printf("%d + %d = %d\n",a,b,a + b);
#endif
	
	return 0;
}
