#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#define HTABLE_DEFAULT_CAPACITY 10//定义哈希表的默认值
#define HTABLE_INSERT_REPLACE 0//覆盖已存在数据的标识
#define HTABLE_INSERT_NOSAME 1//不允许重复的标识
#define HTABLE_INSERT_SAME 2//允许重复数据的标识
						
typedef void (*htable_op)(const void *);
typedef int (*htable_cmp)(const void *,const void *);//用于哈希数据比较

typedef unsigned int (*htable_hash)(const void *,int);//用于计算哈希索引

typedef void HTABLE;

/*
功能：创建哈希表
参数：szie每个数据节点的数据域大小
		capacity 哈希表容量
返回值：成功返回哈希表结构的地址；失败返回空
*/
HTABLE *htable_create(int size,int capacity);

int htable_insert(HTABLE *handler,const void *data,htable_hash hash,htable_cmp cmp,int mode);

void htable_display(HTABLE *handler,htable_op op);

void htable_destroy(HTABLE *handler);



#endif
