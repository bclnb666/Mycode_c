#include <stdio.h>

void xiangzi(int *arr,int size);

void fenpei(int num,const int *arr,int size);

int main(void){
	int num=0;
	int arr[10]={0};
	int size;
	size=sizeof(arr)/sizeof(arr[0]);
	printf("要取的cpu数目");
	scanf("%d",&num);
	xiangzi(arr,size);
	fenpei(num,arr,size);
	printf("\n");
		return 0;
	}

void xiangzi(int *arr,int size)
{
	int sum=1;
	arr[0]=1;
	for(int i = 1;i<=size - 1;i++){
		arr[i]=arr[i-1]*2;
		sum+=arr[i-1];
	}
	arr[size-1] = 1000 - sum + 1;
	printf("10个箱子的存储为：\n");
	for(int i =0;i<size;i++){
		printf("arr[%d]= %d\n",i,arr[i]);
	}
	printf("\n");
}

void fenpei(int num,const int *arr,int size)
{

	printf("取箱子的存储为：\n");
	for(int i =size-1;i>=0;i--){
		if(num>=arr[i]){
			num-=arr[i];
			printf("arr[%d] = %d\n",i,arr[i]);
		}
	}
}
