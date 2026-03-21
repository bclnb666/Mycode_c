#ifndef _MYTEST_LIST_H
#define _MYTEST_LIST_H
/*
offsetof宏求的就是数据域的大小(也就是某个节点从起始地址到某个成员的空间大小)
TYPE : 使用者的结构体类型
MEMBER : 使用者的结构体中某个成员的名字
*/
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
/*
container_of宏 : 求的就是已知结构体中某个成员的地址,通过该地址求结构体的起始地址
ptr : 结构体中某个成员的地址
type : 使用者的结构体类型
member : 使用者的结构体中某个成员的名字
*/

struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

//struct list_head handler = {&handler, &handler};

static inline void INIT_LIST_HEAD(struct list_head *list)
{
list->next = list;
	list->prev = list;
}
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/*
list_for_each宏 : 遍历链表
pos : 指向指针域的指针
head : 指向头节点的指针
*/

#endif
