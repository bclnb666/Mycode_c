#include <stdio.h>

void print(int *B)
{
	*B = 8888;
}



int main(void)
{
	int a = 0;
	int *A = &a;

	printf("%d\n",*A);
	print(A);
	printf("%d\n",*A);
	return 0;
}

