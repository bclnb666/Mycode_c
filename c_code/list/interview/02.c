//删除链表节点
//给你一个无头单向不循环链表的首节点l和一个数据val,
//需要把该链表中所有等于val的节点删除掉,并返回新的首节点

struct llist_node
{
int val;
struct llist_node *next;
};

struct llist_node *func(struct llist_node *l, int val)
{
	struct llist_node *cur = l;
	struct llist_node *prev = NULL;
	for(;cur != NULL;){
		if(cur->val == val){
			if(prev == NULL){
				l = l->next;
				free(cur);
				cur = l;
			}else{
				prev->next = cur->next;
				free(cur);
				cur = prev->next;
			}
		}else{
			prev = cur;
			cur = cur->next;
		}
	
	}
}

