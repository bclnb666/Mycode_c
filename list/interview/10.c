//环形链表
//给你一个单向的链表的首节点 handler 判断链表中是否存在环状结构
//(循环链表也属于环状结构,属于特殊的环状结构,首尾相连)
struct llist_node
{
int val;
struct llist_node *next;
};
int func(struct llist_node *handler)
{
	struct llist_node *f = handler, *s = handler;

	while(f != NULL && f->next != NULL)
	{
		f = f->next->next;
		s = s->next;

		if(f == s)
			return 1;
	}
	return 0;
}
