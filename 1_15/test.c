#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p = NULL;//使用指针p指向开辟的空间
    int i = 0;//循环变量

    p = malloc(5 * sizeof(int));
    //p = calloc(5, sizeof(int));//通过calloc(3)开辟了5个int类型成员的空间
    if(p == NULL)//判断开辟空间是否失败
    {
        fprintf(stderr, "calloc() Is Failed!\n");//打印错误信息
        return -1;//由于开辟空间失败,结束程序,并且返回-1
    }

    for(i = 0; i < 5; i++)
    {
        //p[i] = 100 + i;
        *p = 100 + i;
        printf("*p = %d\n", *p);
        p++;
    }

    free(p-5);//通过free(3)释放开辟的空间
    //指针p已经自增过了,不再指向开辟空间的首地址
    p = NULL;//为了避免使用野指针,把野指针转变为空指针

    return 0;
}
