

#include<stdio.h>

#define VERSION 12

int main()
{
#if VERSION < 1
#warning "版本过低，请充钱升级！"
	int arr[10];
#else
	int arr[20];
#endif

	printf("sizeof(arr) = %ld\n",sizeof(arr));

	return 0;
}


