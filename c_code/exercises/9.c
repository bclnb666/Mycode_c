//个位数为6且能被3整除但不能被5整除的三位自然数共有多少个，分别是哪些？

#include<stdio.h>

int main()

{
	int num=0;
	printf("分别是：");
	for(int i=100;i<=996;i++){
		if(i%10==6&&i%3==0&&i%5!=0){
			printf("%d ",i);
			num++;
		}
	}
	printf("共有%d个\n",num);
	return 0;
}
