/*
 * =====================================================================================
 *
 *    Filename:  test.c
 *
 * Description:  配合filter程序，用于测试消息队列的发送。模拟web， 或comm程序。
 *
 *    
 *
 *     Version:  0.1
 *     Created:  14 Mar 2017  14:20:09
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   14 Mar 2017  14:20:09 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/message.h"
#include "msgQ.h"


/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现



int main(int argc, char *argv[])
{
  int msgID = create_dataQueue();
  debug_msg("msgID = %d", msgID);

  char command[20] = {0};
  char type[2] = {0};
  char text[100] = {0};

  while (1) {                   /* 测试从命令行输入指令 */
    bzero(command, 20);
    printf("please input a command:");
    gets(command);


    if (strcmp(command, "s") == 0) {
      bzero(type, 2);
      bzero(text, 100);
      printf("please input msg type:");
      gets(type);
      if (strlen(type) > 1) {
        printf("wrong type, please re-input");
        continue;
      }
      printf("atol(%s) = %d\n", type, atol(type));
      if (atol(type) < 1 || atol(type) > 9) {
        printf("wrong type, please re-input");
        continue;
      }

      printf("please input msg text:");
      gets(text);

      struct myMsgBuf msgbuf;
      send_message(msgID, &msgbuf, atol(type), text);

    }else if (strcmp(command , "r") == 0) {
      bzero(type, 2);
      bzero(text, 100);
      printf("please input msg type:");
      gets(type);
      if (strlen(type) > 1) {
        printf("wrong type, please re-input");
        continue;
      }
      printf("atol(%s) = %d\n", type, atol(type));
      if (atol(type) < 1 || atol(type) > 9) {
        printf("wrong type, please re-input");
        continue;
      }

      struct myMsgBuf msgbuf;
      read_message(msgID, &msgbuf, atol(type));
      printf("the first msgbuf with msg type = %d, is : %s\n", atol(type), msgbuf.msg_text);

    }

    else if(strcmp(command, "q") == 0) {
      printf("break!\n");
      break;
    }
  }

  return 0;
}
