#include<stdio.h>

char *mystrcpy(char *dest,const char *src)
{
	char *p = dest;
	while(*dest++ = *src++);

	return p;
}

int main()
{
	char dest[10]="hello";
	char *src = "world";

	printf("dest=%s\n",dest);

	

	printf("dest=%s\n",mystrcpy(dest,src));
}
