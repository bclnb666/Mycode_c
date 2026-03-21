//拆分链表
//现有一个无头单向不循环链表的首节点 l 给了一个 val 值,
//将小于 val 值的节点排在剩余节点之前,不能改变链表原来的顺序,
//返回重新整理过的链表的首节点
struct llist_node
{
int val;
struct llist_node *next;
};
struct llist_node *func(struct llist_node *l, int val)
{
	struct llist_node *cur = l;
	struct llist_node *minhead = NULL;
	struct llist_node *maxhead = NULL;
	struct llist_node *mincur = NULL;
	struct llist_node *maxcur = NULL;

	maxhead = malloc(sizeof(struct llist_node));
	minhead = malloc(sizeof(struct llist_node));

	maxhead = minhead = NULL;

	mincur = minhead;
	maxcur = maxhead;
	
	while(cur != NULL)
	{
		if(cur->val < val)
		{
			mincur->next = cur;
			mincur = mincur->next;
		}else
		{
			maxcur->next = cur;
			maxcur = maxcur->next;
		}
		cur = cur->next;
	}

	mincur->next = maxhead->next;
	maxcur->next = NULL;

	l = minhead->next;

	free(maxhead);
	free(minhead);
	
	return l;
}
