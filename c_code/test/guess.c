#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void guess();

int main()
{

	guess();
	
	return 0;
}

void guess()
{
	int num;
	srand(time(NULL));
	int flag = rand() % 100 + 1;
	printf("请输入你要猜的数是几(1-100)：");
	scanf("%d",&num);
	while(1){
		if(num<flag)
			printf("猜小了,请再猜：\n");
		if(num>flag)
			printf("猜大了,请再猜：\n");
		if(num==flag){
			printf("恭喜你猜对了！\n");
			break;
		}
		scanf("%d",&num);
	}
}
