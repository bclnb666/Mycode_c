#include<stdio.h>

void maopao(int *p,int size)
{
	 for(int i=0;i<size-1;i++){
		for(int j=0;j<size-1-i;j++){
			if(p[j]>p[j+1]){
				int temp = p[j];
				p[j]=p[j+1];
				p[j+1]=temp;
			}
		 }
	 }
}

int main()
{	
	int arr[5]={55,23,4,87,66};
	int size=sizeof(arr)/sizeof(arr[0]);
	for(int i=0;i<5;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
	maopao(arr,size);
	
	for(int i=0;i<5;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
	
	return 0;
}
