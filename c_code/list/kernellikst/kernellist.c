#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define NAMESIZE 20

typedef struct stu
{
    int id;//数据域
    char name[NAMESIZE];//数据域
    int math;//数据域
    struct list_head node;//指针域
}STU;

int main(void)
{
    LIST_HEAD(handler);//创建头节点
    STU *data = NULL;//data指针指向开辟的空间
    STU *datap = NULL;//datap指针指向数据节点的首地址
    struct list_head *pos = NULL;//pos指针指向每一个数据节点的指针域
    int i = 0;//循环变量
	int find_id = 103;
	char *delete_name = "stu2";

    for(i = 0; i < 5; i++)
    {
        data = malloc(sizeof(STU));//开辟空间
        if(data == NULL)//判断开辟空间是否失败
            break;//由于开辟空间失败,跳出循环
        data->id = 100 + i;
        sprintf(data->name, "stu%d", i);
        data->math = 100 - i;
        //list_add(&data->node, &handler);//插入数据节点(头插法)
        list_add_tail(&data->node, &handler);//插入数据节点(尾插法)
    }

    //遍历链表(千万不要加 ; 因为这个宏是一个循环,加 ; 会变成空循环)
    list_for_each(pos, &handler)
    {
        datap = list_entry(pos, STU, node);//找到数据节点的首地址
        printf("%d  %s  %d\n", datap->id, datap->name, datap->math);//打印数据
    }

    list_for_each(pos, &handler)
    {
        datap = list_entry(pos, STU, node);//找到数据节点的首地址
		if(datap->id == find_id){
        	printf("找到了：%d  %s  %d\n", datap->id, datap->name, datap->math);//打印数据
			break;
		}
    }

    list_for_each(pos, &handler)
    {
        datap = list_entry(pos, STU, node);//找到数据节点的首地址
		if(datap->name == delete_name){
        	list_del(datap->name);//把数据节点从链表中摘除
        	free(datap);//释放数据节点

			break;
		}
    }

    list_for_each(pos, &handler)
    {
        datap = list_entry(pos, STU, node);//找到数据节点的首地址
        printf("%d  %s  %d\n", datap->id, datap->name, datap->math);//打印数据
    }

    //销毁链表
    list_for_each(pos, &handler)
    {
        datap = list_entry(pos, STU, node);//找到数据节点的首地址
        list_del(pos);//把数据节点从链表中摘除
        free(datap);//释放数据节点
        pos = &handler;//pos指针指向头节点
    }

    return 0;
}
