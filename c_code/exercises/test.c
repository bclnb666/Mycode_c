#include<stdio.h>

int main()
{    
	int i,num = 0;
	scanf("%d",&i);
	while(i){
		if(i%2==1){
			num++;
		}
		i/=2;
	}

	printf("%d\n",num);
    return 0;
}

