#include <stdio.h>
#include "joseph.h"

int main(void)
{   
    JOSEPH *j = NULL;//j指针指向约瑟夫环
    int people_num = 0;//people_num存储总人数
    int kill_num = 0;//kill_num存储数到几杀人
    int alive = 0;//alive存储活下来的人的ID

    printf("请输入总人数 : ");
    scanf("%d", &people_num);
    
    j = joseph_insert(people_num);//根据总人数创建约瑟夫环
    if(j == NULL)//判断创建约瑟夫环是否失败
        return -1;//由于创建约瑟夫环失败,结束程序,并且返回-1
    
    joseph_display(j);//遍历约瑟夫环
    
    printf("请输入数到几杀人 : ");
    scanf("%d", &kill_num);

    alive = joseph_play(j, people_num, kill_num);//开始杀人
    printf("\n======================\n");
    printf("Alive : %d\n", alive);
    
    return 0;
}
