#include<stdio.h>

void myitoa(int data,char str[]) {

	int i = 0;
    int sign = data;
    if (sign < 0) {
        data = -data;
    }
    if (data == 0) {
        str[i++] = '0';
    }
    while (data) {
        str[i++] = data % 10 + '0';
        data /= 10;
    }
    if (sign < 0) {
        str[i++] = '-';
    }
    str[i] = '\0';
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - 1 - j];
        str[len - 1 - j] = temp;
    }
}


int main()
{
	char str[100]={0};
	int num;
	scanf("%d",&num);
	myitoa(num,str);
	printf("%s\n", str);
	return 0;
}
