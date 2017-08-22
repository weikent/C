/*
 * =====================================================================================
 *
 *    Filename:  TcpClient.c
 *
 * Description:  接受本地层数据。发送给服务器或者进行通讯层处理。
 *
 *    
 *
 *     Version:  0.1
 *     Created:  Thu Apr 17 10:14:34 2014
 *
 *     Authors:  世建 魏 ,  
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   世建 魏  Thu Apr 17 10:14:34 2014 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */


#include "TcpClient.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <linux/if.h>
#include <sys/time.h>
#include <netdb.h>
#include <pthread.h>

#include "packMessage.h"
#include "SendSocket.h"
#include "global.h"
#include "../../common/message.h"
#include "tool.h"
#include "ConnectionCheck.h"
#include "ReadSocketBuffer.h"
#include "../../common/cJSON.h"





typedef struct threadPar
{
  unsigned char *str; 
}threadPar;

/* 
 * ===  FUNCTION  ========================================================================
 *         Name: filterLocalData
 *  Description: 过滤本地层的数据。
 *   Parameters: 
 /// @param data 
 *      Created: Fri Apr 11 16:39:03 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Fri Apr 11 16:39:03 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

void filterLocalData(const char *data)
{

}


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: readLocalMessage
 *  Description: 读取本地层数据。阻塞式。
 *   Parameters: 
 /// @param str 值-value变量
 * 
 * Return Value: 
 *      Created: Thu Apr 17 10:25:30 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 10:25:30 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int readLocalMessage(char **str)
{
  struct myMsgBuf msgReadMsgBuf;

  bzero(msgReadMsgBuf.msg_text, sizeof(msgReadMsgBuf.msg_text));
  read_message(msgID, &msgReadMsgBuf, MY_MSG_TYPE);

  debug_msg("the message is : %s,  the sender is : %d", msgReadMsgBuf.msg_text, msgReadMsgBuf.sender_type);

  *str = malloc(strlen(msgReadMsgBuf.msg_text) + 1);
  bzero(*str, strlen(msgReadMsgBuf.msg_text) + 1);
  memcpy(*str, msgReadMsgBuf.msg_text, strlen(msgReadMsgBuf .msg_text));

  /* char messageType[2]; */

  /* sprintf(messageType, "%d", tmsg.msg_type); */

  return strlen(msgReadMsgBuf.msg_text);
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: TcpClientRun
 *  Description: 线程函数，主要用来获取本地层数据，过滤数据，完善system层数据，发送给服务器。
 *   Parameters: 
 /// @param arg 
 * 
 * Return Value: 
 *      Created: Fri Apr 11 16:40:19 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Fri Apr 11 16:40:19 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */
void *TcpClientRun(void *arg)
{
  for ( ;  ;  )
    {
      unsigned char *str = NULL;
      int len = readLocalMessage(&str);

      if (deviceModel == AP) {
        free(str);
        continue;
      }

      if (!g_isSubscribed) {
        continue;
      }
      char *result = NULL;
      len = mqttPublish(len, str, &result);
      if (g_serverIP[0] != 0 && g_serverPort != 0)
        {
          sendSocket(result, len, "str");
        }
      else  // if not get server IP and port
        {
        }

      free(result);
      free(str);
    } // for loop end
}


