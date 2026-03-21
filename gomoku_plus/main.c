#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gomoku.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>//有关framebuffer的头文件
#include <sys/mman.h>

int fb = 0;//fb变量存储打开framebuffer设备的文件描述符
int ret = 0;//ret变量用来接收函数的返回值
struct fb_var_screeninfo var;//定义了可变屏幕信息的变量
struct fb_fix_screeninfo fix;//定义了固定屏幕信息的变量
unsigned char *pc = NULL;//指向映射的内存空间
unsigned int *pi = NULL;//指向映射的内存空间
unsigned int bytes = 0;//bytes变量存储1个像素点所占的字节数
unsigned long offset = 0;//offset变量存储的是偏移量

void draw_point(int x, int y, int color)//封装画一个像素点的函数
{
	offset = x * bytes + y * fix.line_length;//计算要画的像素点
	pi = (unsigned int *)(pc + offset);//指针转换
	*pi = color;//给像素点染色
}
void draw_circle(int x, int y, int r, int color)
{
	int i = 0, j = 0;

	for(i = x - r; i <= x + r; i++)
		for(j = y - r; j <= y + r; j++)
			if((i-x)*(i-x) + (j-y)*(j-y) <= r*r)
				draw_point(i, j, color);
}


int main()
{
	fb = open("/dev/fb0", O_RDWR);//通过open(2)以读写的形式打开framebuffer设备
	if(fb < 0)//判断打开framebuffer设备是否失败
	{
		perror("open()");//打印错误信息
		return -1;//由于打开framebuffer设备失败,结束程序,并且返回-1
	}

	ret = ioctl(fb, FBIOGET_VSCREENINFO, &var);//通过ioctl(2)获取可变的屏幕信息
	if(ret < 0)//判断获取可变的屏幕信息是否失败
	{
		perror("ioctl()");//打印错误信息
		close(fb);//通过close(2)关闭打开的framebuffer设备
		return -2;//由于获取可变的屏幕信息失败,结束程序,并且返回-2
	}

	bytes = var.bits_per_pixel >> 3;//计算1个像素点所占字节数

	ret = ioctl(fb, FBIOGET_FSCREENINFO, &fix);//通过ioctl(2)获取固定的屏幕信息
	if(ret < 0)//判断获取固定的屏幕信息是否失败
	{
		perror("ioctl()");//打印错误信息
		close(fb);//通过close(2)关闭打开的framebuffer设备
		return -2;//由于获取固定的屏幕信息失败,结束程序,并且返回-2
	}
	pc = mmap(0, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);//把framebuffer映射到内存中
	if(pc == (void *)(-1))//判断把framebuffer映射到内存中是否失败
	{
		perror("mmap()");//打印错误信息
		close(fb);//通过close(2)关闭打开的framebuffer设备
		return -3;//由于把framebuffer映射到内存失败,结束程序,并且返回-3
	}

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

