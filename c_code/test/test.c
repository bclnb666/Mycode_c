#include <stdio.h>

int main(void)
{
    char *arr[3] = {"easthome", "hello", "world"};
    //arr[0]相当于是char *类型的指针,arr[0]存储的是"easthome"字符串常量的首地址
    //arr[1]相当于是char *类型的指针,arr[1]存储的是"hello"字符串常量的首地址
    //arr[2]相当于是char *类型的指针,arr[2]存储的是"world"字符串常量的首地址
    char brr[3][10] = {"easthome", "hello", "world"};

    int i = 0;//循环变量

    for(i = 0; i < 3; i++)
        printf("arr[%d] = %s\n", i, arr[i]);

    for(i = 0; i < 3; i++)
        printf("brr[%d] = %p\n", i, brr[i]);
	int a = 88;
	int *my = &a;
	printf("my=%d\n",*my);

    printf("sizeof(arr) = %ld\n", sizeof(arr));
    printf("sizeof(brr) = %ld\n", sizeof(brr));

    //arr[2][1] = 'O';不可以,arr[2]指向了常量区,不能修改指向空间的数据
    brr[2][1] = 'O';//可以

    arr[2] = "123456789";//可以
    printf("arr[2] = %s\n", arr[2]);

   // brr[2] = "123456789";//不可以
    printf("brr[2] = %s\n", brr[2]);

    return 0;
}

