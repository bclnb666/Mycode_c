#include<stdio.h>


int main()
{
	char str;
	int space = 1;
	while((str = getchar())!='\n'){
		if(str==' '){
			space++;
		}
	}
	printf("单词的个数：%d\n",space);

	return 0;
}
