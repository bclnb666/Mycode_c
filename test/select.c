#include<stdio.h>

void select(int *p,int size)
{
	 for(int i=0;i<size-1;i++){
		int k = i;
		for(int j=i+1;j<size;j++){
			if(p[j]<p[k]){
				k = j;
			}
		 }
		int temp = p[i];
		p[i]=p[k];
		p[k]=temp;
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
	select(arr,size);
	
	for(int i=0;i<5;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
	
	return 0;
}
