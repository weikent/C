/**
 * @file   main.c
 * @brief  this is the main file of platform2(7688 chip)
 *
 * <long description> 测试版本，用来测试7688平台的可行性。
 *
 * @author weikent
 * @date   2017-03-09
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "global.h"
#include "../../common/msgQ.h"
#include "socketSerialPort.h"
#include "timer.h"


#include "../../common/message.h"



void init(){
  msgID = create_dataQueue();
  debug_msg("msgID = %d", msgID);

  sem_init(&g_semHasCommand, 0, 0);
  sem_init(&g_semNeedWait, 0, 0);


  init_serial();
}


int main(int argc, char *argv[])
{

  init();
  char *buf = malloc(MAX_SERIAL_MSG_SIZE + 1);
  /* TODO: 暂时先在这里一次读取一个命令，
           以后要写一个缓存，读取命令之后， 统一放到缓存之后再处理。
           成功放到消息队列之后，再从缓存中删除这个命令
   */
  struct myMsgBuf msgReadMsgBuf;

  int temp;
  pthread_t pt_timer;

  if((temp = pthread_create(&pt_timer, NULL, timer, NULL)) != 0)
    {
    }
  else
    {
    }

  while(1){

    bzero(msgReadMsgBuf.msg_text, sizeof(msgReadMsgBuf.msg_text));
    read_message(msgID, &msgReadMsgBuf, MY_MSG_TYPE);

    debug_msg("the message is : %s,  the sender is : %d", msgReadMsgBuf.msg_text, msgReadMsgBuf.sender_type);
    bzero(buf, MAX_SERIAL_MSG_SIZE + 1);
    uart_send(serial_fd, msgReadMsgBuf.msg_text, 1); /* TODO:  需要先解析消息队列中的信息，然后再发送 */

    sem_post(&g_semHasCommand);
    sem_wait(&g_semNeedWait);

    int len = uart_recv(serial_fd, buf, MAX_SERIAL_MSG_SIZE);
    if (len <= 0) {
      debug_msg("uart_recv failed, maybe timeout");
      continue;
    }

    buf[len+1] = '\0';
    debug_msg("buf = %s\n", buf);

    int sender = msgReadMsgBuf.sender_type;
    msgReadMsgBuf.sender_type = MY_MSG_TYPE;
    strncpy(msgReadMsgBuf.msg_text, buf, len);
    msgReadMsgBuf.msg_type = sender;
    send_message(msgID, &msgReadMsgBuf, sender, buf);
    /* TODO:  解析此buf， */
    /* TODO:  send msg */
  }

  return 0;
}

