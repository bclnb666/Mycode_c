#include <stdio.h>

int main()
{
    int a = 100;
    int b = 66;
    int temp;

    // temp = a;
    // a = b;
    // b = temp;

    a = a + b;
    b = a - b;
    a = a - b;

	printf("%d %d\n",a,b);
	
	return 0;
}