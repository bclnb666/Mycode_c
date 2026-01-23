#include <stdio.h>

void itob(int a);

int main()
{
	int a;
	printf("请输入要转二进制的十进制数：");
	scanf("%d",&a);
	itob(a);
	return 0;
}

void itob(int num)
{
	int arr[8]={0};
	int flag =0;
	int a = num;
	while(num){
		arr[flag]=num%2;
		num/=2;
		flag++;
	}
	printf("%d的二进制为：",a);
	for(int i=flag-1;i>=0;i--)
		printf("%d",arr[i]);
	printf("\n");
}
