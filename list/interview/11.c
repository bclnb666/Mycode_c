//找到环的入口点
//给你一个单向的链表的首节点 handler 判断链表中是否存在环状结构,如果是环状结构,找到环的入口点
//(循环链表也属于环状结构,属于特殊的环状结构,首尾相连)
struct llist_node
{
int val;
struct llist_node *next;
};

struct llist_node *func(struct llist_node *handler)
{
	struct llist_node *f = handler;
	struct llist_node *s = handler;

	struct llist_node *h = handler;

	while(f != NULL && f->next != NULL)
	{
		f = f->next->next;
	   	s = s-next;
		if(s == f)
		{
			while(s != h)
			{
				s = s->next;
				h = h->next;
			}
			return s;
		}
	
	}

	return NULL;
}
