//链表的回文结构
//现有一个无头单向不循环链表的的首节点 l 最后返回一个真假值,
//如果链表是一个回文结构就返回真值,如果链表不是回文结构就返回假值(如果链表只存在一个节点也算回文结构)
struct llist_node
{
int val;
struct llist_node *next;
};
int func(struct llist_node *l)
{
	struct llist_node *f = l , *s = l;
	struct llist_node *p = NULL;

	while(f != NULL && f->next != NULL)
	{
		f = f->next->next;
		s = s->next;
	}

	f = s->next;
	s->next = NULL;
	
	while(f != NULL)
	{
		p = f->next;
		f->next = s;
		s = f;
		f = p;
	}
	f = l;
	while(f != NULL && s != NULL)
	{
		if(f->val != s->val)
			return 0;
		s = s->next;
		f = f->next;
	}
	return 1;

}
