#include<stdio.h>

size_t mystrlen(char *s)
{
	int i =0;
	for(i=0;s[i]!='\0';i++);
	
	return i;	
}

int main()
{
	char dest[10]="hello";
	char *src = "world";

	printf("dest=%s\n",dest);

	

	printf("dest=%ld\n",mystrlen(dest));
}
