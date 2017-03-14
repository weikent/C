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
#include "msgQ.h"
#include "socketSerialPort.h"


void init()
{
  msgID = create_dataQueue();
  debug_msg("msgID = %d", msgID);


  sem_init(&g_semHasCommand, 0, 0);
  sem_init(&g_semNeedWait, 0, 0);


  init_serial();
}




int main(int argc, char *argv[])
{
  /* char command[20] = {0}; */
  /* char type[2] = {0}; */
  /* char text[100] = {0}; */

  /* while (1) {                   /\* 测试从命令行输入指令 *\/ */
  /*   bzero(command, 20); */
  /*   printf("please input a command:"); */
  /*   gets(command); */


  /*   if (strcmp(command, "s") == 0) { */
  /*     bzero(type, 2); */
  /*     bzero(text, 100); */
  /*     printf("please input msg type:"); */
  /*     gets(type); */
  /*     if (strlen(type) > 1) { */
  /*       printf("wrong type, please re-input"); */
  /*       continue; */
  /*     } */
  /*     printf("atol(%s) = %d\n", type, atol(type)); */
  /*     if (atol(type) < 1 || atol(type) > 9) { */
  /*       printf("wrong type, please re-input"); */
  /*       continue; */
  /*     } */

  /*     printf("please input msg text:"); */
  /*     gets(text); */

  /*     struct mymsgbuf msgbuf; */
  /*     send_message(msgID, &msgbuf, atol(type), text); */

  /*   }else if (strcmp(command , "r") == 0) { */
  /*     bzero(type, 2); */
  /*     bzero(text, 100); */
  /*     printf("please input msg type:"); */
  /*     gets(type); */
  /*     if (strlen(type) > 1) { */
  /*       printf("wrong type, please re-input"); */
  /*       continue; */
  /*     } */
  /*     printf("atol(%s) = %d\n", type, atol(type)); */
  /*     if (atol(type) < 1 || atol(type) > 9) { */
  /*       printf("wrong type, please re-input"); */
  /*       continue; */
  /*     } */

  /*     struct mymsgbuf msgbuf; */
  /*     read_message(msgID, &msgbuf, atol(type)); */
  /*     printf("the first msgbuf with msg type = %d, is : %s\n", atol(type), msgbuf.msg_text); */

  /*   } */

  /*   else if(strcmp(command, "q") == 0) { */
  /*     printf("break!\n"); */
  /*     break; */
  /*   } */
  /* } */

  char *buf = malloc(MAX_SERIAL_MSG_SIZE + 1);
  /* TODO: 暂时先在这里一次读取一个命令，
           以后要写一个缓存，读取命令之后， 统一放到缓存之后再处理。
           成功放到消息队列之后，再从缓存中删除这个命令
   */
  struct myMsgBuf msgReadMsgBuf;


  while(1){

    read_message(msgID, &msgReadMsgBuf, MY_MSG_TYPE);

    debug_msg("the message is : %s", msgbuf.msg_text);



    bzero(buf, MAX_MSG_TEXT_SIZE + 1);
    uart_send(serial_fd, "a", 1); /* TODO:  替换成实际需要的命令， */
    sem_post(&g_semHasCommand);
    sem_wait(&g_semTimer);
    uart_recv(serial_fd, buf, MAX_SERIAL_MSG_SIZE);
    /* TODO:  解析此buf， */
    /* TODO:  send msg */
  }

  return 0;
}

