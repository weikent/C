/*
 * =====================================================================================
 *
 *    Filename:  DeviceModelChange.c
 *
 * Description:  used to check the model of this device.
 *               if model = AP, just send message to arduino.
 *               if model = STATION, start all thread.
 *
 *
 *
 *     Version:  0.1
 *     Created:  05 May 2017  15:32:15
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   05 May 2017  15:32:15 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件
#include "DeviceModelChange.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

/*------------Local Variables----------- */
//定义一些本地变量



/*------Local Structures and Typedefs ---*/
//要使用的一些数据结构



/*-----------Extern Variables -----------*/
//使用到的一些外部变量



/*-------------Definitions---------------*/
//一些#defines及具体的函数实现


void *DeviceModelChange(void *arg){

  debug_msg("start DeviceModelChange......");

  for ( ;  ;  )
    {

      sleep(5);
      char   *buf;
      buf = malloc(20);
      memset( buf, '\0', 20);

      char command[100] = {0};
      sprintf(command, "uci get wireless.sta.disabled");

      int a = getCommandResult(command, &buf);
      printf("sta.disabled = %s", buf);


      if (strcmp(buf, "0") == 0) {
        deviceModel = STATION;
      }else{
        deviceModel = AP;
        g_isConnected = false;
        g_isCreated = false;
        g_isSubscribed = false;
      }
      free(buf);
    }
}
