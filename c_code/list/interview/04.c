/链表的中间节点
//给你一个头节点为handler的单向不循环链表,
//返回链表中的中间节点,如果有两个中间节点返回第二个中间节点
struct llist_node
{
int val;
struct llist_node *next;
};
//快慢指针方法
struct llist_node *func(struct llist_node *handler)
{
	struct llist_node *f = handler;
	struct llist_node *s = handler;

	while(f != NULL && f->next != NULL){
		f = f->next->next;
		s = s->next;
	}
	return s;
}

