
/* 
 *     Created:  17 May 2013 13:56:54
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
#include "msgqueue.h"
#include "global/global.h"


int send_message(int qid, struct mymsgbuf *qbuf, long type, char *text)
{
    printf ("Sending a message....\n");
    qbuf->mtype = type;
    strcpy(qbuf->mtext, text);

    if ((msgsnd(qid, (struct msgbuf *)qbuf,strlen(qbuf->mtext) + 1, 0)) == -1)
    {
	return -1;
    }
    return 0;
}


int read_message(int qid, struct mymsgbuf *qbuf, long type)
{
    printf ("Reading a message....\n");
    qbuf->mtype = type;
    if ((msgrcv(qid, (struct msgbuf *) qbuf, MAX_SEND_SIZE, type , 0)) == -1)
    {
	return -1;
    } 

    printf ("Type: %ld Text: %s \n", qbuf->mtype, qbuf->mtext);

    return 0;
}


int remove_queue(int qid)
{
    if ((msgctl(qid, IPC_RMID, 0)) == -1)
    {
	return -1;
    }
    return 0;
}

int create_queue()
{
    key_t key;
    int msgqueue_id;

    key = ftok("~", 'm');
	
    if ((msgqueue_id = msgget(key, IPC_CREAT | 0660)) == -1)
    {
	return -1;
    }
    return msgqueue_id;
}
