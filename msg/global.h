/* @(#)global.h
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h> 

#define MAXLINE 1024
#define MAX_SEND_SIZE 80



struct mymsgbuf
{
    long mtype;
    char mtext[MAX_SEND_SIZE];
};


#endif /* _GLOBAL_H */

