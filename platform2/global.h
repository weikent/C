/**
 *   \file global.h
 *   \brief 一些全局变量的定义
 *
 *  一些全局变量的定义
 *
 *
 * Fri Mar 10 14:36:39 2017
 *
 * author:魏世建
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <pthread.h>
#include <semaphore.h>




#define DEBUG 1			/* switch of print debug info */

#ifdef DEBUG
#define debug_msg(args...) do                                           \
    {                                                                   \
      fprintf(stdout, "%-30s[%4d]_%-20s : ", __FILE__, __LINE__, __FUNCTION__); \
      fprintf(stdout, args);                                            \
      fprintf(stdout, "\n");                                            \
    }                                                                   \
  while(0)
#else
#define debug_msg(arg...) ((void)0)
#endif



extern sem_t g_semNeedWait;               // 用于计时，
extern sem_t g_semHasCommand;


#define SERIAL_PORT "/dev/ttyS0"
#define MAX_MSG_TEXT_SIZE 10000 // 消息队列多能传送的最大字符串
#define MAX_SERIAL_MSG_SIZE 100 // 串口所能接收的最大字符
#define MY_MSG_TYPE 3           // 这个程序的消息队列的type

typedef struct myMsgBuf {
  long msg_type;
  char msg_text[TEXT_SIZE];
} MsgData;
extern int msgID;

#endif /* GLOBAL_H */
