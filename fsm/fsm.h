#ifndef __FSM_H
#define __FSM_H

//宏定义buf空间的大小
#define BUFSIZE 1024

//定义状态
enum {STATE_R, STATE_W, STATE_E, STATE_T};

//定义有限状态机的数据类型
typedef struct
{   
    int rfd;            //读文件的文件描述符
    int wfd;            //写文件的文件描述符
    char buf[BUFSIZE];  //存储读取到的数据
    int count;          //存储成功读取到的字节个数
    int pos;            //存储已写入的字节个数
    int state;          //存储有限状态机当前的状态
    char *errmsg;       //记录出错函数的函数名
}fsm_t;

//定义接口
/*
功能 : 初始化有限状态机
参数 : f 要初始化的有限状态机
        rfd 读文件的文件描述符(客户指定)
        wfd 写文件的文件描述符(客户指定)
返回值 : 成功返回0;失败返回<0
[1]给有限状态机开辟空间
[2]给有限状态机中的每个成员进行初始化
注意 : 那么如何保证客户传递过来 rfd wfd 是非阻塞的?
可以使用fcntl(2)先获取原有的文件状态,按位或上O_NONBLOCK,再设置回去
*/
extern int fsm_init(fsm_t **f, int rfd, int wfd);

/*
功能 : 推动有限状态机
参数 : f 指向要推动的有限状态机
返回值 : 成功返回0;失败返回<0
判断有限状态机的状态
[R] 把rfd文件里的数据独到buf中,判断read(2)返回值
    >0  读成功了,推到[W]
    ==0 EOF,推到[T]
    <0  读错误了,判断是不是假错,如果是假错重读,推到[E]
[W] 把buf里面存储的数据写道wfd文件(判断是不是把要写入的数据都写入了,续写)
[E] perror(3)打印错误信息,推到[T]
[T] 终止进程
*/
extern int fsm_drive(fsm_t *f);

/*
功能 : 销毁有限状态机
参数 : f 指向要销毁的有限状态机
返回值 : 成功返回0;失败返回<0
*/
extern int fsm_destroy(fsm_t *f);

#endif
