//输出倒数第 k 个节点
//给你一个头节点为handler的单向不循环链表,输出链表的倒数第 k 个节点
struct llist_node
{
int val;
struct llist_node *next;
};
//快慢指针方法
struct llist_node *func(struct llist_node *handler, int k)
{
	struct llist_node *f = handler, *s = handler;

	while(k--)
	{
		if(f == NULL)
			return NULL;
		f = f->next;
	}

	while(f != NULL)
	{
		f = f->next;
		s = s->next;
	}

	return s;
}
