#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void init(char qp[15][15]);//初始化棋盘
void printqp(char qp[15][15],int flag,int count1,int count2);//打印棋盘
void writein(char qp[15][15],int *x,int *y,int *flag,int *count1,int *count2);//读取坐标输入并写入棋盘+选手回合计数
void iflegal(char qp[15][15],int *x,int *y);//进行输入并判断所下位置是否合法
bool ifwin(char qp[15][15],int x,int y,bool *ifdraw);//判断是否获胜

int main()
{
    char qp[15][15] = {0};//定义二维数组存储棋盘
    int x, y, flag,count1=0,count2=0;//flag用来判断选手，count用来记录选手已下回合数
    bool gameover = false;//判断游戏是否结束
    bool ifdraw = false;
    
    init(qp);//初始化
    
    printf("请先选择谁先手:(1-玩家一 2-玩家二)");
    while (scanf("%d",&flag)!=1||(flag!=1&&flag!=2)) {//判断输入是否合法
        printf("输入错误！请输入1或2: ");
        while (getchar()!='\n');//清空输入缓冲区
    }
    
    while (!gameover) {

        printqp(qp,flag,count1,count2);
        iflegal(qp, &x, &y);
        writein(qp, &x, &y, &flag,&count1,&count2);
        
        if (ifwin(qp, x, y, &ifdraw)) {
            printqp(qp,flag,count1,count2);
            if (ifdraw) {
                printf("\n平局！棋盘已满，游戏结束。\n");
            } else if (flag == 1) {  // 注意：writein已经切换了flag
                printf("\n恭喜玩家二获胜！\n");
            } else {
                printf("\n恭喜玩家一获胜！\n");
            }
            gameover = true;
        }
    }
    
    return 0;
}
void init(char qp[15][15])
{
	for(int i=0;i<=15;i++)
		for(int i=0;i<15;i++){
			for(int j=0;j<15;j++){
			qp[i][j]='+';
		}
	}
}
void printqp(char qp[15][15],int flag,int count1,int count2)
{
	system("clear");//清屏
	for(int i=0;i<=15;i++)
		printf("%X ",i);
	printf("      五子棋\n");
	//打印棋盘+插入提示符
	for(int i=0;i<15;i++){
		printf("%X ",i+1);
		for(int j=0;j<15;j++){
			printf("%c ",qp[i][j]);
		}
		if(i==2)printf("    双方交替下棋");
		if(i==3)printf("    先连成五子的");
		if(i==4)printf("    一方即为胜利");
		if(i==6){
			if(flag==1)printf("    当前回合:玩家一");
			else printf("    当前回合:玩家二");
		}
		if(i==9){
			printf("    玩家一:0 已下%d手",count1);
		}
		if(i==10){
			printf("    玩家二:X 已下%d手",count2);
		}

		printf("\n");
	}

}

void writein(char qp[15][15],int *x,int *y,int *flag,int *count1,int *count2)
{
	 if(*flag==1){
		qp[*x-1][*y-1]='0';
		*flag=2;
		(*count1)++;
		}
		else{
			qp[*x-1][*y-1]='X';
			*flag=1;
			(*count2)++;
		}
}

void iflegal(char qp[15][15], int *x,int *y)
{
    int valid_input = 0;
    while (!valid_input) {
		printf("请输入要下的坐标:(x,y)");
		if (scanf("%d,%d", x,y) == 2) {
            valid_input = 1;  // 输入成功
			if(qp[*x-1][*y-1]!='+'){
			printf("!警告:该位置已有棋子，请重新输入！\n");
			valid_input = 0;
		}

        } else {
			printf("!警告:输入不合法，请重新输入！\n");
            // 清空输入缓冲区
            while (getchar() != '\n');  // 读取并丢弃所有字符直到换行符
        }
    }
}
/*
六连一般分为三种情况：
标准规则，只要五连就赢，六连视为两个五连
有禁手规则下，六子算长连禁手，对手可以直接宣告胜利（或者你直接被判负）
六不赢规则下，连六的棋型不算输也不算赢，大于五连视为无效，双方继续走棋
值得注意的是，任何规则共同的一点——只要形成五连，就算赢，无论是否同时形成了任何其他棋型。
补充一下，布局方法和禁手是两个概念，连换、三手，这些叫做布局规则（布局方法），有禁手、六不赢，这些叫做禁手规则，注意区分
 */
bool ifwin(char qp[15][15],int x,int y,bool *ifdraw)//六连也判胜
{
    int row=x-1;
    int col=y-1;
    char current_player=qp[row][col];
    
    if (current_player=='+') {
        return false;
    }
    
    // 检查棋盘是否已满（平局）
    bool board_full=true;
    for(int i=0;i<15&&board_full;i++) {
        for (int j = 0;j<15;j++) {
            if(qp[i][j]=='+') {
               board_full=false;
               break;
            }
        }
    }
    
    if (board_full) {
        *ifdraw= true;
        return true;  // 游戏结束（平局）
    }
    
    int count = 0;
    
    // 水平方向
    count = 1;
    for(int i=col-1;i>=0&&qp[row][i]==current_player;i--) 
		count++;
    for(int i=col+1;i<15&&qp[row][i]==current_player;i++) 
		count++;
    if(count>=5) 
		return true;
    // 垂直方向
    count = 1;
    for(int i=row-1;i>=0&&qp[i][col]==current_player;i--) 
		count++;
    for(int i=row+1;i<15&&qp[i][col]==current_player;i++) 
		count++;
    if (count>=5) 
		return true;
    
    // 主对角线
    count = 1;
    for (int i=row-1,j=col-1;i>=0&&j>=0&&qp[i][j]==current_player;i--,j--) 
		count++;
    for (int i=row+1,j=col+1;i<15&&j<15&&qp[i][j]==current_player;i++,j++) 
		count++;
    if (count >= 5) 
		return true;
    
    // 副对角线
    count = 1;
    for(int i=row-1,j=col+1;i>=0&&j<15&&qp[i][j]==current_player;i--,j++) 
		count++;
    for(int i=row+1,j=col-1;i<15&&j>=0&&qp[i][j]==current_player;i++,j--) 
		count++;
    if(count >= 5) 
		return true;
    
    return false;
}

