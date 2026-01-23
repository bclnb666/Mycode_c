#include <stdio.h>

int atoi(char x)
{
    char *p = &x;
	int num = 0;

	for(;*p!='\0';p++){

		num*=10;

		num+=*p - '0';
	}
	return num;
}

int main()
{
	char x;
	scanf("%c",&x);
	printf("%d\n",atoi(x));

	return 0;
}
