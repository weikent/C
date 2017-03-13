// -*- C++ -*-

/* @(#)msgQ.h
 */

#ifndef _MSGQ_H
#define _MSGQ_H 1
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "global.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text);
  void read_message(int qid, struct mymsgbuf *qbuf, long type);
  int create_dataQueue();
#ifdef __cplusplus
}
#endif
#endif /* _MSGQ_H */

