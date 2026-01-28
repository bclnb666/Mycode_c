#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
};

struct tree_node
{
	struct tree_node *r;
	struct tree_node *l;
	struct stu data;
};

static struct tree_node *tree = NULL;

void tree_insert(struct tree_node **root,struct stu data)
{
	if(*root == NULL){
		*root = malloc(sizeof(struct tree_node));
		if(*root == NULL)
			return;
		(*root)->r = (*root)->l = NULL;
		(*root)->data = data;
		return;
	}
	if((*root)->data.id < data.id)
		tree_insert(&(*root)->r,data);
	else
		tree_insert(&(*root)->l,data);
}

void print(struct stu data)
{
	printf("%d %s %d\n",data.id,data.name,data.math);
}

void tree_display(struct tree_node *root)
{
	if(root == NULL)
		return;
	tree_display(root->l);
	tree_display(root->r);
	print(root->data);
}

void tree_distroy(struct tree_node *root)
{
	if(root == NULL)
		return;
	tree_distroy(root->l);
	tree_distroy(root->r);
	free(root);
}
struct stu *tree_find(struct tree_node *root,int find_id)
{
	if(root == NULL)
		return NULL;
	if(root->data.id == find_id)
		return &root->data;
	else if(root->data.id > find_id)
		return tree_find(root->l,find_id);
	else
		return tree_find(root->r,find_id);
}

struct tree_node *find_max(struct tree_node *root)
{
	if(root == NULL)
		return NULL;
	if(root->r == NULL)
		return root;
	return find_max(root->r);
}

struct tree_node *find_min(struct tree_node *root)
{
	if(root == NULL)
		return NULL;
	if(root->l == NULL)
		return root;
	return find_min(root->l);
}

int tree_delete(struct tree_node **root,int find_id)
{
	struct tree_node **node = root;
	struct tree_node *cur = NULL;
	
	while(*node != NULL && (*node)->data.id != find_id){
		if((*node)->data.id < find_id)
			node = &(*node)->r;
		else
			node = &(*node)->l;
	}
	if(*node == NULL)
		return -1;
	cur = *node;
	if(cur->l == NULL)
		*node = cur->r;
	else{
		*node = cur->l;
		find_max(cur->l)->r = cur->r;
	}
	free(cur);
	return 0;
	
}
void _draw(struct tree_node *root, int level)
{
    int i = 0;//循环变量

    if(root == NULL)//判断当前以root指向的节点为根节点,是否为空树
        return ;//由于当前以root指向的节点为根节点是空树,结束函数
    _draw(root->r, level + 1);
    for(i = 0; i < level; i++)
        printf("  ");
    printf("%d\n", root->data.id);
    _draw(root->l, level + 1);
}

void tree_draw(struct tree_node *root)
{
    system("clear");//清屏
    _draw(root, 0);
    getchar();
}

int getnum(struct tree_node *root)
{
	if(root == NULL)
		return 0;
	return 1 + getnum(root->l) + getnum(root->r);
}

void turnleft(struct tree_node **root)
{
	struct tree_node *node = *root;
	*root = node->r;
	node->r = NULL;
	find_min(*root)->l = node;
	tree_draw(tree);
}

void turnright(struct tree_node **root)
{
	struct tree_node *node = *root;
	*root = node->l;
	node->l = NULL;
	find_max(*root)->r = node;
	tree_draw(tree);
}

void tree_balance(struct tree_node **root)
{
	int num = 0;
	if(*root == NULL)
		return;
	while(1){
		num = getnum((*root)->l) - getnum((*root)->r);
		if(num >= -1 && num <= 1)
			break;
		else if(num < -1)
			turnleft(root);
		else
			turnright(root);
	}
	tree_balance(&(*root)->l);
	tree_balance(&(*root)->r);
}

int main()
{
	int arr[] = {1,2,3,7,6,5,4,9,8};
	struct stu data;
	struct stu *datap = NULL;
	int find_id = 7;
	for(int i = 0;i < sizeof(arr) / sizeof(*arr);i++){
		data.id = arr[i];
		sprintf(data.name,"Ammon%d",arr[i]);
		data.math = 60;
		tree_insert(&tree,data);
	}	
	
	tree_display(tree);
	printf("----------------------------\n");
	datap = tree_find(tree,find_id);
	if(datap == NULL)
		printf("No Find It\n");
	else{
		printf("Find It:");
		print(*datap);
	}
	printf("----------------------------\n");

	int ret = tree_delete(&tree,find_id);
	if(ret != 0)
		printf("Dleete It Failed!\n");
	else{
		printf("Delete It Succeed!\n");
		tree_display(tree);
	}
	
	tree_draw(tree);

	tree_balance(&tree);

	tree_distroy(tree);


	return 0;
}
