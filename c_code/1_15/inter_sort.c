#include<stdio.h>

void inter_sort(int *arr,int size)
{
	if(size<=1)
		return;

	for(int i=1;i<size;i++){
		int j=i-1;
		int flag = arr[i];
		for(;j>=0&&arr[j]>flag;j--){
			arr[j+1]=arr[j];
		}
		arr[j+1]=flag;
	}
	
}

int main()
{
	int arr[5] = {100,45,55,7,304};
	
	for(int i=0;i<5;i++)
		printf("%d ",arr[i]);
	printf("\n");

	inter_sort(arr,5);

	for(int i=0;i<5;i++)
		printf("%d ",arr[i]);
	printf("\n");

	return 0;
}
