#include <stdio.h>

float avg(int *p,int size);

void print(const int *p,int size,float a);


int main()
{
	int stu_arr[10];
	int size = sizeof(stu_arr)/sizeof(stu_arr[0]);
	for(int i = 0;i<size;i++){
		scanf("%d",&stu_arr[i]);
	}
	float avrage = avg(stu_arr,size);
	printf("平均分为：%f\n",avrage);
	printf("小于平均分的是：");
	print(stu_arr,size,avrage);
	return 0;
}

float avg(int *p,int size)
{
	int sum=0;
	for(int i=0;i<size;i++){
		sum+=p[i];
	}
	return sum/size;
}

void print(const int *p,int size,float a)
{
	for(int i=0;i<size;i++){
		if(p[i]<a)
			printf("%d ",p[i]);
	}
	printf("\n");
}
