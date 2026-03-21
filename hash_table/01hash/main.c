#include <stdio.h>
#include <string.h>
#include "hashtable.h"

#define NAMESIZE 20

struct stu//客户数据的结构
{   
    int id;
    char name[NAMESIZE];
    int math;
    int tel;
};

void print(const void *data)
{   
    const struct stu *p = data;//转换指针
    printf("%d  %s  %d  %d-> ", p->id, p->name, p->math, p->tel);
}

int id_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{   
    const struct stu *p = data;//转换指针
    const int *k = key;//转换指针
    
    return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{   
    const struct stu *p = data;//转换指针
    const int *k = key;//转换指针
    
    return !(p->tel - *k);
}

int name_cmp(const void *data, const void *key)//data是数据域的数据 key是要找的数据
{   
    const struct stu *p = data;//转换指针
    const char *k = key;//转换指针
    
    return !strcmp(p->name, k);
}
/*
功能：基于学生ID的哈希函数，计算数据应存入的索引值
参数：data指向学生结构体的指针
		capacity哈希表的容量
返回值：计算出哈希值
 */

unsigned int id_hash(const void *data,int capacity)
{
	const struct stu *p = data;//转换指针
	return p->id % capacity;
}

unsigned int tel_hash(const void *data,int capacity)
{
	const struct stu *p = data;//转换指针
	return p->tel % capacity;
}

unsigned int name_hash(const void *data,int capacity)
{
	const char *name = ((const struct stu*)(data))->name;//转换指针
	unsigned int hash = 0;//存储哈希索引值
	
	while(*name)
	{
		//字符串哈希函数 hash * 31 + 字符ASCII值
		hash = hash * 31 + *name++;
	}

	return hash % capacity;
}

int main(void)
{	
	HTABLE *ht = NULL;//ht指针指向哈希表的结构
    struct stu data;


	ht = htable_create(sizeof(struct stu),8);//创建哈希表（每个节点数据域的大小，哈希表的容量）
	if(ht == NULL)
	{
		fprintf(stderr,"htable_create Is Failed!\n");
		return -1;
	}

	for(int i = 0; i < 10; i++)
	{
		data.id = i;
		sprintf(data.name,"Ammon%d",i);
		data.math = 99;
		data.tel = 1833395 + i;
		htable_insert(ht,&data,id_hash,id_cmp,HTABLE_INSERT_REPLACE);
	}

	htable_display(ht,print);

	htable_destroy(ht);



    return 0;
}
