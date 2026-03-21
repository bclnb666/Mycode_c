#include <stdio.h>

static int mycp(const char *pathname1,char *pathname2)
{
	FILE *fp1 = NULL,*fp2 = NULL;
	int ch = 0;

	fp1 = fopen(pathname1,"r");
	fp2 = fopen(pathname2,"w");
	
	int fse = fseek(fp1,50,SEEK_SET);
	if(fse != 0)
		printf("fseek() Is Failed!\n");

	if(fp1 == NULL && fp2 == NULL)
	{
		perror("fopen()");
		return -1;
	}
	
	while(1)
	{
		ch = fgetc(fp1);
		if(ch == EOF)
		{
			if(ferror(fp1))
			{
				fclose(fp1);
				fclose(fp2);
				return -2;
			}
			break;
		}
		fputc(ch,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"USage : %s + filename\n",argv[0]);
		return -1;
	}
	
	mycp(argv[1],argv[2]);

	return 0;
}
