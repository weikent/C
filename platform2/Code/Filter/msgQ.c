/*
 * =====================================================================================
 *
 *    Filename:  msgQ.c
 *
 * Description:  词文件用于创建消息队列，发送、读取。
 *
 *
 *
 *     Version:  0.1
 *     Created:  13 Mar 2017  13:39:14
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   13 Mar 2017  13:39:14 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 
#include "msgQ.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


void send_message(int qid, struct myMsgBuf *qbuf, long type, char *text)
{
  printf ("Sending a message....\n");
  qbuf->msg_type = type;
  strcpy(qbuf->msg_text, text);

  if ((msgsnd(qid, (struct myMsgBuf *)qbuf,strlen(qbuf->msg_text) + 1, 0)) == -1)
    {
      perror("msgsend");
      exit(1);
    }
}


void read_message(int qid, struct myMsgBuf *qbuf, long type)
{
  printf ("Reading a message....\n");
  qbuf->msg_type = type;
  msgrcv(qid, (struct msgbuf *) qbuf, MAX_MSG_TEXT_SIZE, type , 0);
  //  printf ("Type: %ld Text: %s \n", qbuf->msg_type, qbuf->msg_text);
}


int create_dataQueue()
{
  key_t key;
  int msgqueue_id;

  key = ftok('.', 'a');

  if ((msgqueue_id = msgget(key, IPC_CREAT | 0660)) == -1)
    {
      return -1;
    }
  //    printf("queue_id= %d", msgqueue_id);

  return msgqueue_id;
}

