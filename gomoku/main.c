#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gomoku.h"


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

