//3.从终端（键盘）读入20个数据到数组中，统计其中正数的个数，并计算这些正数之和。

#include<stdio.h>

int main()
{
    int arr[21];
    int sum=0,num=0;
    for(int i=0;i<20;i++){
        scanf("%d",&arr[i]);
    }
    for(int i=0;i<20;i++){
        if(arr[i]>0){
            num++;
            sum+=arr[i];
        }
    }
    printf("正数的个数为：%d,这些正数之和为：%d\n",num,sum);
    return 0;
}
