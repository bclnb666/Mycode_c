#include<stdio.h>

char *mystrcat(char *s1,const char *s2)
{
	int i =0,j =0;
	for(i=0;s1[i]!='\0';i++);
	for(j=0;s2[j]!='\0';j++)
		s1[i++]=s2[j];
	s1[i]=s2[j];
	return s1;
}

int main()
{
	char dest[10]="hello";
	char *src = "world";

	printf("dest=%s\n",dest);

	

	printf("dest=%s\n",mystrcat(dest,src));
}
