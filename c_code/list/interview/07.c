//合并两个有序的链表
//将两个升序的无头单向不循环链表合并为一个新的升序链表并返回,
//新链表是通过拼接两个链表的所有节点组合而成的
struct llist_node
{
int val;
struct llist_node *next;
};
struct llist_node *func(struct llist_node *l1, struct llist_node *l2)
{
	struct llist_node *l = NULL;
	struct llist_node *cur = NULL;

	if(l1 == NULL)
		return l2;
	if(l2 == NULL)
		return l1;


	if(l1->val < l2->val){
		l = cur = l1;
		l1 = l1->next;
	}else{
		l = cur = l2;
		l2 = l2->next;
	}

	while(l1 != NULL && l2 != NULL){
		if(l1->val < l2->val)
		{
			cur->next = l1;
		   	l1 = l1->next;	
		}
		else
		{
			cur->next = l2;
			l2 = l2->next;
		}
		cur = cur->next;
	}

	if(l1 == NULL)
	{
		cur->next = l2;
	}
	else
	{
		cur->next = l1;
	}
	
	return l;
}
