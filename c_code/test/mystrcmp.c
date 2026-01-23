#include<stdio.h>

int mystrcmp(char *s1,const char *s2)
{
	int i =0;
	for(i=0;s1[i]==s2[i]&&s1[i]!='\0';i++);

	return s1[i] - s2[i];	
}

int main()
{
	char dest[10]="hello";
	char *src = "world";

	printf("dest=%s\n",dest);

	

	printf("dest=%d\n",mystrcmp(dest,src));
}
