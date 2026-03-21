#ifndef __GOMOKU_H
#define __GOMOKU_H

void init(char qp[15][15]);

void printqp(char qp[15][15],int flag,int count1,int count2);

void writein(char qp[15][15],int *x,int *y,int *flag,int *count1,int *count2);

void iflegal(char qp[15][15], int *x,int *y);

bool ifwin(char qp[15][15],int x,int y,bool *ifdraw);


#endif
