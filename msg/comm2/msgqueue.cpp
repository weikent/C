
/* 
 *     Created:  17 May 2013 13:56:54
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
#include "msgqueue.h"
//#include "global/global.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h> 

#define MAXLINE 1024
#define MAX_SEND_SIZE 1024



msgqueue::msgqueue()
{
}

msgqueue::~msgqueue()
{
}


int msgqueue::send_message(int qid, struct MsgData *qbuf, long type, char *text)
{
    printf ("Sending a message....\n");

    qbuf->msg_type = type;
    strcpy(qbuf->msg_text, text);


    if ((msgsnd(qid, (struct MsgData *)qbuf,strlen(qbuf->msg_text) + 1, 0)) == -1)
    {
	return -1;
    }
    return 0;
}


int msgqueue::read_message(int qid, struct MsgData *qbuf, long type)
{
    printf ("Reading a message....\n");
    qbuf->msg_type = type;
    if ((msgrcv(qid, (struct MsgData*) qbuf, MAX_SEND_SIZE, type , 0)) == -1)
    {
	return -1;
    } 

    printf ("Type: %ld Text: %s \n", qbuf->msg_type, qbuf->msg_text);

    return 0;
}


int msgqueue::remove_queue(int qid)
{
    if ((msgctl(qid, IPC_RMID, 0)) == -1)
    {
	return -1;
    }
    return 0;
}

int msgqueue::create_queue()
{
    key_t key;
    int msgqueue_id;

    key = ftok("ctl_snd_tpt_rcv", 'a');
	
    if ((msgqueue_id = msgget(key, IPC_CREAT | 0660)) == -1)
    {
	return -1;
    }
    return msgqueue_id;
}
