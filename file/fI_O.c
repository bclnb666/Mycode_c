#include <stdio.h>


struct data
{
	int hum_data;
	int temp_data;
	float pm_data;
	float ch4_data;
	char str[16];
};


int main()
{
struct data d = {
		12,3344,12.34,34.12,"SmartHome"
	};

	FILE *fp = NULL;
	fp = fopen("666.txt","rw");

	if(fp == NULL)
	{
		perror("fopen()");
		return -1;
	}

	fscanf(fp,"%d %d %f %f %s",&d.hum_data,&d.temp_data,&d.pm_data,&d.ch4_data,d.str);

	fprintf(fp, "hum_data:\n",d.hum_data);
    fprintf(fp, "temp_data: %d\n",d.temp_data);
    fprintf(fp, "pm_data: %f\n",d.pm_data);
    fprintf(fp, "ch4_data: %f\n",d.ch4_data);
    fprintf(fp, "str: %s\n",d.str);

    fclose(fp);
	return 0;
}
