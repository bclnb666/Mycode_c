#include <stdio.h>

void itob(int a,int n);

int main()
{
	int a,n;
	printf("请输入想要从十进制转换的进制：");

	scanf("%d",&n);
	printf("请输入十进制数字：");
	scanf("%d",&a);

	itob(a,n);
	return 0;
}

void itob(int a,int n)
{
	while(a){
		printf("%d",a%n);
		a/=n;
	}
	printf("\n");
}
