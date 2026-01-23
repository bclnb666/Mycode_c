#include<stdio.h>

int myatoi(const char *nptr) {
    int res = 0;
    int sign = 1;

    while (*nptr==' ') {
        nptr++;
    }

    if (*nptr == '-') {
        sign = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }

    while (*nptr >= '0' && *nptr <= '9') {
        res = res * 10 + (*nptr - '0');
        nptr++;
    }

    return res * sign;
}


int main()
{
	int num = 0;
	num = myatoi("123");//123
	printf("%d\n",num);
    num = myatoi("abc");//0
	printf("%d\n",num);
    num = myatoi("123abc");//123
	printf("%d\n",num);
    num = myatoi("abc123");//0
	printf("%d\n",num);
    num = myatoi("-123");//-123
	printf("%d\n",num);
    num = myatoi("+123");//123
	printf("%d\n",num);
    num = myatoi("123+456");//123
	printf("%d\n",num);
    num = myatoi("0123");//123
	printf("%d\n",num);
    num = myatoi("    123");//123
	printf("%d\n",num);
    num = myatoi("123   456");//123
	printf("%d\n",num);
	
	return 0;
}
