#include<stdio.h>


int main()
{
	char str;
	int letter = 0,space = 0,num = 0,other = 0;
	while((str = getchar())!='\n'){
		if((str>='a'&&str<='z')||(str>='A'&&str<='Z')){
			letter++;
		}else if(str>='0'&&str<='9'){
			num++;
		}else if(str==' '){
			space++;
		}else{
			other++;
		}
	}
	printf("字母的个数：%d\n",letter);
	printf("空格的个数：%d\n",space);
	printf("数字的个数：%d\n",num);
	printf("其他字符的个数：%d\n",other);

	return 0;
}
