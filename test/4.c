#include <stdio.h>

int main()
{
    int a;
    scanf("%d",&a);

	printf("个位：%d 十位：%d 百位：%d 千位：%d\n",a%10,a/10%10,a/100%10,a/1000);
	
	return 0;
}