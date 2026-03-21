#include <stdio.h>



int main(int argc,char *argv[])
{
	FILE *fp = NULL;
	for(int i = 1;i < argc;i++)
	{
		fp = fopen(argv[i],"w");
		fclose(fp);
		fp = NULL;
	}
	return 0;
}
