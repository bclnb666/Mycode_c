//给你一个无头单向不循环链表的首节点l,
//需要把这个链表反过来,并返回反转之后新的首节点
struct llist_node
{
int val;
struct llist_node *next;
};
struct llist_node *func(struct llist_node *l)
{
	if(l == NULL || l->next == NULL)
		return l;

	struct llist_node *cur = l,*p = l->next;
	l->next = NULL;

	while(p != NULL){
		l = p;
		p = p->next;
		l->next = cur;
		cur = l;
	}

	return l;
}
