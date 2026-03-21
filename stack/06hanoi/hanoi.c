#include <stdio.h>
#include <stdlib.h>

struct hanoi
{
	int hanoi_arr[5];
	int hanoi_top;
}hanoi[3] = {{{5,4,3,2,1},5} ,{},{}};

void draw()
{
	int i = 0,j = 0;
	system("clear");
	for(int i = 0;i < 3;i++){
		for(j = 0;j < hanoi[i].hanoi_top;j++)
			printf("%d",hanoi[i].hanoi_arr[j]);
		for(;j < 10;j++)
			printf("-");
		printf("\n");
	}
	getchar();
}

void swap(int src,int dest)
{
	hanoi[src].hanoi_top--;
	hanoi[dest].hanoi_arr[hanoi[dest].hanoi_top] = hanoi[src].hanoi_arr[hanoi[src].hanoi_top];
	hanoi[dest].hanoi_top++;
	draw();
}

void play(int src,int dest,int tmp,int num)
{
	if(num == 1){
		swap(src,dest);
		return;
	}
	play(src,tmp,dest,num - 1);
	swap(src,dest);
	play(tmp,dest,src,num - 1);
}

int main()
{
	draw();
	play(0,1,2,5);
	


	return 0;
}
