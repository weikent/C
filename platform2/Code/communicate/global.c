/*
 * =====================================================================================
 *
 *    Filename:  global.c
 *
 * Description:  全局变量
 *
 *
 *
 *     Version:  0.1
 *     Created:  17 Mar 2017  09:58:01
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   17 Mar 2017  09:58:01 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件
#include "global.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



_Bool g_isCreated;
_Bool g_isConnected;
_Bool g_isSubscribed;
_Bool g_isListChanged;
//sem_t g_semSendHeart;


char **topicList;
int topicListCount;

//msgNode *messageArr;



pthread_mutex_t g_pthDealmap;
pthread_mutex_t g_pthDealPubAck; /* 处理发送puback的锁 */


char messageType[2];      /* 从local层获取的消息的类型 */


char *g_subscribeTopicFilter[] = {"isockets/Server/", "isockets/Web/", "isockets/Topic/"};
short g_subscribePacketIdentifier;


pthread_mutex_t g_pthTCPSocket;

sem_t g_semConnectionCheck;
sem_t g_semNetcom;



int clientModelTCPSocket = -1;


_Bool shouldCloseSocket = false;



_Bool isChecking = false;

_Bool selectTimeout = false;
_Bool hasSendData = false;
_Bool isFirstUse = false;

char g_mac[LEN_OF_MAC];
char g_serverIP[LEN_OF_IPADDRESS] = {0};
int g_serverPort = 0;


int msgID = 0;
