//4.从终端（键盘）将5个整数输入到数组a中，然后将a逆序复制到数组b中，并输出b中各元素的值。

#include<stdio.h>

int main()
{
    int arr1[6],arr2[6];
    for(int i=0;i<5;i++){
        scanf("%d",&arr1[i]);
    }
    for(int i=0,j=4;i<5;i++,j--){
        arr2[i]=arr1[j];
    }
    printf("b中各元素的值为：");
    for(int i=0;i<5;i++){
        printf("%d ",arr2[i]);
    }
    printf("\n");
    return 0;
}
