#include<stdio.h>
#include<string.h>

char* jiami(char *str)
{
	for(int i=0;i<strlen(str);i++){
		if(str[i]>='y'&&str[i]<='z') 
			str[i]-=24;
		else if(str[i]>='a'&&str[i]<='x') 
				str[i]+=2;
		else
			str[i]+=0;
	}

	return str;
}


int main()
{
	char str[100];
	scanf("%s",str);
	
	printf("加密前：%s\n",str);
	jiami(str);
	printf("加密后：%s\n",str);
	
	return 0;
}
