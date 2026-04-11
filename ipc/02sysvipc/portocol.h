#ifndef __PORTOCOL_H
#define __PORTOCOL_H

#define PATH    "/etc/passwd"
#define PROJ_ID 'a'

#define STRSIZE 1024

struct msg_st
{

    long mtype;
    char str[STRSIZE];
};


#endif
