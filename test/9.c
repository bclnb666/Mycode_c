#include <stdio.h>

int main()
{
	for(int i =1000;i<10000;i++){
		int j = i*9;
		if(i/1000==j%10 && i/100%10==j/10%10 && i/10%10==j/100%10 && i%10==j/1000){
			printf("%d\n",i);
			break;
		}
	}
	return 0;
}
