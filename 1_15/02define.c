#include<stdio.h>

#define SUB

int main()
{

	int a = 13,b = 7;


#ifdef SUB
	printf("%d - %d = %d\n",a,b,a - b);
#else
	printf("%d + %d = %d\n",a,b,a + b);
#endif
	
	return 0;
}
