#include <stdio.h>

#define SIZE 101

static int mycat(const char *pathname)
{
	FILE *fp = NULL;
	char ch[SIZE];

	fp = fopen(pathname,"r");
	if(fp == NULL)
	{
		perror("fopen()");
		return -1;
	}
	
	while(1)
	{
		if(fgets(ch,SIZE,fp) == NULL)
		{
			if(ferror(fp))
			{
				perror("fgets()");
				fclose(fp);
				return -2;
			}
			break;
		}
		fputs(ch,stdout);
	}
	fclose(fp);
	return 0;


}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"USage : %s + filename\n",argv[0]);
		return -1;
	}
	
	mycat(argv[1]);

	return 0;
}
