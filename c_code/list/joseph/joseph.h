#ifndef __JOSEPH_H
#define __JOSEPH_H

typedef struct llist_node
{
    int id;
    struct llist_node *next;
}JOSEPH;
    
JOSEPH *joseph_insert(int p_n);
    
void joseph_display(JOSEPH *j);
    
int joseph_play(JOSEPH *j, int p_n, int k_n);

#endif
    

