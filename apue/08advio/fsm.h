#ifndef __FSM_H
#define __FSM_H

#define BUFSIZE 1024

enum {STATE_R,STATE_W,STATE_E,STATE_T};
      
 typedef struct
{
    int rfd;
    int wfd;
    char buf[BUFSIZE];
    int count;
    int pos;
    int state;
    char *errmsg;

}fsm_t;



#endif
