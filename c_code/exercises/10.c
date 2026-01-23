//输入某年某月某日，判断这一天是这一年的第几天？

#include<stdio.h>

int main()
{
	int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int i,y,m,d,day=0;
	scanf("%d %d %d",&y,&m,&d);
	if(y%400==0 || (y%100!=0&&y%4==0)) //判断闰年 
	{
		month[2]+=1;
	}
	for(i=0;i<m;i++)
	{
		day+=month[i];
	}
	day+=d;
	printf("这是今年的第%d天\n",day);
	return 0;
}

