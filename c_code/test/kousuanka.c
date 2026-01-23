#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void add(int a,int b);
void sub(int a,int b); 
void mul(int a,int b);
void div_b(int a,int b); 
void rem(int a,int b); 
	
int score=0;

int main(void)

{
	int i=0,a=0,b=0,c=0;
	srand(time(NULL));
	for(i=0;i<10;i++){
		a=rand()%100+1;
		b=rand()%100+1;
		c=rand()%5;
		switch(c){
			case 0: add(a,b); break;
			case 1: sub(a,b); break;
			case 2: mul(a,b); break;
			case 3: div_b(a,b); break;
			case 4: rem(a,b); break;
		}
	printf("目前的分数:%d\n",score);
	}
	printf("总得分:%d\n",score);
	return 0;
}

void add(int a,int b)
{
	printf("%d + %d=",a,b);
	int num;
	scanf("%d",&num);
	if(num==a+b){
		printf("Bingo! +10分！\n");
		score+=10;
	}else
		printf("Error！real is %d\n",a+b);
}
void sub(int a,int b)
{
	printf("%d - %d=",a,b);
	int num;
	scanf("%d",&num);
	if(num==a-b){
		printf("Bingo! +10分！\n");
		score+=10;
	}else
		printf("Error！real is %d\n",a-b);
}
void mul(int a,int b)
{
	printf("%d * %d=",a,b);
	int num;
	scanf("%d",&num);
	if(num==a*b){
		printf("Bingo! +10分！\n");
		score+=10;
	}else
		printf("Error！real is %d\n",a*b);
}
void div_b(int a,int b)
{
	printf("%d / %d=",a,b);
	int num;
	scanf("%d",&num);
	if(num==a/b){
		printf("Bingo! +10分！\n");
		score+=10;
	}else
		printf("Error！real is %d\n",a/b);
}
void rem(int a,int b)
{
	printf("%d %% %d=",a,b);
	int num;
	scanf("%d",&num);
	if(num==a%b){
		printf("Bingo! +10分！\n");
		score+=10;
	}else
		printf("Error！real is %d\n",a%b);
}

