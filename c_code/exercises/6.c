//求n以内（不包括n）同时能被3和7整除的所有自然数之和的平方根s，n从键盘输入。例如若n为1000时，函数值应为：s=153.909064。

#include<stdio.h>
#include <math.h>

int main()
{
    int n;
	double sum=0;
    scanf("%d",&n);
    for(int i=21;i<n;i++){
        if(i%3==0&&i%7==0){
			sum+=i;
		}
    }
	printf("S=%lf\n",sqrt(sum));
    return 0;
}
