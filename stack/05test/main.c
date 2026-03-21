#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int get_priority(char op)
{   
    switch(op)
    {
        case '(' : return 0;//优先级低
        case '+' : 
        case '-' : return 1;//优先级中
        case '*' : 
        case '/' : return 2;//优先级高
        default : return -1;
    }
}

int infix_to_postfix(const char *infix, char *postfix)
{
    STACK *op_stack = NULL;//op_stack指针指向运算符的栈
    int i = 0, j = 0;//循环变量
    int op = 0;//存储出栈的运算符
    int cur_op = 0;
    int top_op = 0;
    int len = strlen(infix);//获取中缀表达式的字符串长度

    op_stack = stack_create();//开辟运算符栈的空间
    if(op_stack == NULL)//判断运算符栈开辟是否失败
    {
        fprintf(stderr, "stack_create() Is Failed!\n");//打印错误信息
        return -1;//由于运算符栈开辟失败,结束函数,并且返回-1
    }

    while(i < len)//循环遍历中缀表达式
    {
        if(infix[i] == ' ')//跳过中缀表达式中的空格
        {
            i++;
            continue;
        }
        //[1]当前字符是数字字符
        if(infix[i] >= '0' && infix[i] <= '9')
        {
            //循环处理多位数 
            while(i < len && infix[i] >= '0' && infix[i] <= '9')
            {   
                postfix[j++] = infix[i++];//把中缀表达式中的数字字符写入后缀表达式
            }
            postfix[j++] = ' ';//用空格隔开数字和运算符
        }//[2]当前字符是左括号 ( 
        else if(infix[i] == '(')
        {   
            stack_push(op_stack, infix[i]);//左括号入栈
            i++;//索引自增
        }//[3]当前字符是右括号 )
        else if(infix[i] == ')')
        {
            //循环出栈,直到遇到左括号为止
            while(!is_empty(op_stack))
            {
                stack_pop(op_stack, &op);//出栈运算符
                if(op == '(')
                    break;
                postfix[j++] = op;//运算符写入到后缀表达式中
                postfix[j++] = ' ';////用空格隔开数字和运算符
            }
            i++;//索引移动,跳过右括号
        }//[4]当前运算符是加减乘除运算符
        else if(strchr("+-*/", infix[i]))
        {
            cur_op = infix[i];//存储当前的运算符
            while(!is_empty(op_stack))
            {
                stack_pop(op_stack, &top_op);//弹出栈顶运算符
                //判断当前中缀表达式中的运算符是否大于栈顶运算符的优先级
                if(get_priority(top_op) < get_priority(cur_op))
                {
                    stack_push(op_stack, top_op);//把刚才出栈的运算符再入栈
                    break;
                }
                postfix[j++] = top_op;//把栈顶运算符写入后缀表达式
                postfix[j++] = ' ';//用空格隔开数字和运算符
            }
            stack_push(op_stack, cur_op);//把当前中缀表达式中的运算符入栈
            i++;//索引移动
        }//[5]不认识的字符
        else
        {
            printf("出现错误的字符 : %c\n", infix[i]);//打印
            stack_destroy(op_stack);//销毁栈
            return -2;//由于出现了错误的字符,结束函数,并且返回-2
        }
    }
    //遍历结束了中缀表达式,把栈中的数据全弹出
    while(!is_empty(op_stack))
    {
        stack_pop(op_stack, &op);//出栈
        //判断出栈的是否不是 + - * /
        postfix[j++] = op;
        postfix[j++] = ' ';
    }

    postfix[j-1] = '\0';//把后缀表达式中的最后的空格替换成'\0'
    stack_destroy(op_stack);//销毁栈
    return 0;
}

int repolish(const char *postfix, int *result)
{
    STACK *num_stack = NULL;//num_stack指针指向数据的栈
    int len = strlen(postfix);//计算后缀表达式的长度
    int i = 0;//循环变量
    int num = 0;//num变量用来存储转换以后的整数
    int num1 = 0, num2 = 0;//分别表示第一个操作数和第二个操作数

    num_stack = stack_create();//开辟数据栈的空间
    if(num_stack == NULL)//判断开辟数据栈的空间是否失败
    {
        fprintf(stderr, "stack_create() Is Failed!\n");//打印错误信息
       return -1;//由于开辟数据栈的空间失败,结束函数,并且返回-1
   }
   while(i < len)//循环变量后缀表达式
   {
       if(postfix[i] == ' ')//跳过后缀表达式中的空格
       {
           i++;
           continue;
       }
       //[1]当前字符是数字
       if(postfix[i] >= '0' && postfix[i] <= '9')
       {
           num = 0;//清空num值
           while(i < len && postfix[i] >= '0' && postfix[i] <= '9')
           {
               num = num * 10 + (postfix[i] - '0');//把数字字符转换成整数
               i++;//索引后移
           }
           stack_push(num_stack, num);//把转换后的整数入栈
       }//[2]当前字符是加减乘除运算符
       else if(strchr("+-*/", postfix[i]))
       {
           if(stack_pop(num_stack, &num2) == -1)//弹出第二个操作数
           {
               printf("表达式有问题,(操作数少了)\n");
               stack_destroy(num_stack);//销毁栈
               return -2;//由于第二个操作数出栈失败,结束函数,并且返回-2
           }
           if(stack_pop(num_stack, &num1) == -1)//弹出第一个操作数
           {
               printf("表达式有问题,(操作数少了)\n");
               stack_destroy(num_stack);//销毁栈
               return -3;//由于第一个操作数出栈失败,结束函数,并且返回-3
           }
           switch(postfix[i])
           {
               case '+' : stack_push(num_stack, num1 + num2); break;
               case '-' : stack_push(num_stack, num1 - num2); break;
               case '*' : stack_push(num_stack, num1 * num2); break;
               case '/' : if(num2 == 0)//判断除数是否为0
                           {
                               printf("表达式中除数为0\n");
                               stack_destroy(num_stack);//销毁栈
                               return -4;//由于表达式中除数为0,结束函数,并且返回-4
                           }
                           stack_push(num_stack, num1 / num2); break;
               default : break;
           }
           i++;//索引后移
       }
   }
   if(stack_pop(num_stack, result) == -1)
   {
       printf("表达式有问题!\n");
       stack_destroy(num_stack);//销毁栈
       return -5;//由于最终结果出栈失败,结束函数,并且返回-5
    }

    if(!is_empty(num_stack))
    {
        printf("表达式有问题,(操作数过多)!\n");
        stack_destroy(num_stack);//销毁栈
        return -6;//由于表达式操作数过多,结束函数,并且返回-6
    }

    stack_destroy(num_stack);//销毁栈
    return 0;
}

int main(void)
{
    char infix[32] = {0};//存储客户录入的中缀表达式
    char postfix[32] = {0};//存储转换以后的后缀表达式
    int ret = 0;//ret变量用来存储函数的返回值
    int result = 0;//result变量用来存储后缀表达式的计算结果

    //[1]客户录入中缀表达式
    printf("请输入中缀表达式(表达式中可以录入空格) : ");
    fgets(infix, sizeof(infix) ,stdin);//从标准输入流中获取字符串存储到infix数组中
    //strcspn(3):计算字符串长度,返回的是从字符串开头到第一个匹配字符的位置
    infix[strcspn(infix, "\n")] = '\0';//把fgets(3)获取的'\n'替换成'\0'
    printf("infix = %s\n", infix);

    //[2]把中缀表达式转换成后缀表达式
    ret = infix_to_postfix(infix, postfix);
    if(ret != 0)//判断转换是否失败
    {
        fprintf(stderr, "infix_to_postfix() Is Failed!\n");//打印错误信息
        return -1;//由于转换失败,结束函数,并且返回-1
    }
    printf("转换之后的,后缀表达式 : %s\n", postfix);
    ret = repolish(postfix, &result);//计算结果
    if(ret == 0)//判断计算的结果是否失败
        printf("result = %d\n", result);

    return 0;
}

