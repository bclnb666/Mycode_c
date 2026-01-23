#include<stdio.h>
#include<stdlib.h>
#include <time.h>

void iflegal(int argc,char *argv[]);

void print(int month,int year);

int cailei(int month,int year);

int main(int argc,char *argv[])
{
	iflegal(argc,argv);
	return 0;
}

void iflegal(int argc,char *argv[])
{	
	time_t now = time(NULL);
    // 转换为本地时间结构体
    struct tm *local = localtime(&now);
    int now_year = local->tm_year + 1900;  // tm_year是从1900年开始的年份数
    int now_month = local->tm_mon + 1;     // tm_mon范围是1-12，1表示1月

	if(argc==1){
		print(now_month,now_year);
	}else{
		int month = atoi(argv[1]);
		int year = atoi(argv[2]);
		if(month>12||month<0){
			printf("cal: %d together with a given year but no given month is not supported.\n",month);
		}else if(year>9999||year<0){
			printf("cal: year `%d' not in range 1..9999\n",year);
			}else if(year==0){
				printf("cal: not %s valid year %s\n",argv[2],argv[2]);
			}else{
				print(month,year);
			}
		}
}

void print(int month,int year)
{
	int day=0;
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
		day=31;
	else if(month==2&&(year%4==0&&year%100!=0||year%400==0))
			day=29;
		else if(month==2&&(!(year%4==0&&year%100!=0||year%400==0)))
				day=28;
			else
				day=30;
	printf("      %d月 %d\n",month,year);
	printf("日 一 二 三 四 五 六\n");
	int h = cailei(month,year);
	//printf("%d\n",h);
	int arr[6][7]={0};
	switch(h){
		case 0:break;
		case 1:printf("  ");break;
		case 2:printf("     ");break;
		case 3:printf("        ");break;
		case 4:printf("           ");break;
		case 5:printf("              ");break;
		case 6:printf("                 ");break;
	}
	int i =0,flag=1;
	for(i = 1;i<=7-h;i++){
		if(h==0&&i==1)
			printf("%2d",i);
		else
			printf("%3d",i);
	}
	printf("\n");
	for(i;i<=day;i++){
		if(i<10){
			printf(" ");
		}
		printf("%d ",i);
		flag++;
		if(flag%8==0){
			printf("\n");
			flag=1;
		}
	}
	printf("\n");
}

int cailei(int month,int year)
{
	if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (1 + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    // 转换为常见的表示：0=星期日，1=星期一，...，6=星期六
    return (h + 6) % 7;
}
