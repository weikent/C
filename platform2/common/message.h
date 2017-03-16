// -*- C++ -*-

/* @(#)message.h
 */

#ifndef _MESSAGE_H
#define _MESSAGE_H 1

#ifdef __cplusplus
extern "C"
{
#endif
#define MAX_MSG_TEXT_SIZE 8000 // 消息队列多能传送的最大字符串

  typedef struct myMsgBuf {
    long int msg_type;
    long int sender_type;
    char msg_text[MAX_MSG_TEXT_SIZE];
  } MsgData;

#ifdef __cplusplus
}
#endif


#endif /* _MESSAGE_H */

