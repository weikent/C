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
#include "../../common/message.h"
#include "../../common/msgQ.h"



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


extern int g_shmid;             //   share memory
extern struct shared_use_st *g_sharedRead;      //
extern struct shared_use_st *g_sharedWrite;     // 
extern unsigned char g_sharedWriteBuf[SHARED_TEXT_SIZE + 1];
extern pthread_mutex_t g_pthSharedWrite;


//extern unsigned char g_serialMessageBuf[MAX_SERIAL_MSG_SIZE + 1];

extern struct myMsgBuf g_msgReadMsgBuf;
extern struct myMsgBuf g_msgWriteMsgBuf;
extern pthread_mutex_t g_pthMsgWrite;

extern sem_t g_semNeedWait;               // 用于计时，
extern sem_t g_semHasCommand;


#define SERIAL_PORT "/dev/ttyS0"



#define MAX_SERIAL_MSG_SIZE 100 // 串口所能接收的最大字符
#define MY_MSG_TYPE 3           // 这个程序的消息队列的type
#define COMM_MSG_TYPE 1

extern int msgID;

#endif /* GLOBAL_H */
