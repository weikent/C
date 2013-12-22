/* @(#)msgqueue.h
 */

#ifndef _MSGQUEUE_H
#define _MSGQUEUE_H 1

#include "global/global.h"

int create_queue();
int send_message(int qid, struct mymsgbuf *qbuf, long type, char *text);
int read_message(int qid, struct mymsgbuf *qbuf, long type);

int remove_queue(int qid);

#endif /* _MSGQUEUE_H */

