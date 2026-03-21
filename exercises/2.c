//2.计算1+2+3…+n的值，n是从键盘输入的自然数。

#include<stdio.h>

int main()
{
    int n,sum=0;
    scanf("%d",&n);
    for(int i=n;i>0;i--)
    {
        sum+=i;
    }
    printf("%d\n",sum);
    return 0;
}
