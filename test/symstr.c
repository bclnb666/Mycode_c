#include<stdio.h>
#include<string.h>
#include<stdbool.h>
bool ifsym(char *str)
{
	int i = 0,j = strlen(str);

	if(j%2==0) return false;
	int z = j-1;
	for(;str[i]==str[z]&&i!=z;i++,z--);
	if(i==z)
		return true;
	else
		return false;
}


int main()
{
	char str[100];
	scanf("%s",str);
	if(ifsym(str))
		printf("%s是对称串\n",str);
	else
		printf("%s不是对称串\n",str);
	
	return 0;
}
