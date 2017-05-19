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




int clientModelTCPSocket = -1;


_Bool shouldCloseSocket = false;



_Bool g_isChecking = false;

_Bool selectTimeout = false;
_Bool hasSendData = false;
_Bool isFirstUse = false;

char g_mac[LEN_OF_MAC];
char g_serverIP[LEN_OF_IPADDRESS] = {0};
int g_serverPort = 0;
char g_serverWebsite[LEN_OF_WEBSITE] = {0};

/* mac */
char g_mac[LEN_OF_MAC] = {0};
// wifi info
char g_ssid[LEN_OF_WIFIINFO] = {0};
char g_password[LEN_OF_WIFIINFO] = {0};

// dhcp info
int g_dhcpStatus = -1;
char g_ip_addr[LEN_OF_IPADDRESS] = {0};
char g_mask[LEN_OF_IPADDRESS] = {0};
char g_gateWay[LEN_OF_IPADDRESS] = {0};
char g_dns1[LEN_OF_IPADDRESS] = {0};
char g_dns2[LEN_OF_IPADDRESS] = {0};


int msgID = 0;

enum deviceModel deviceModel = AP;
enum deviceStatus deviceStatus = JOIN_ROUTE_FAILED;

unsigned char key1[] = "12345678";
unsigned char key2[] = "abcdefgh";
unsigned char key3[] = "12345678";

unsigned char key1API[] = "12345678";
unsigned char key2API[] = "abcdefgh";
unsigned char key3API[] = "12345678";

_Bool isReceived3DesChange = false;
_Bool is3DesKeyChanged = false;


char *mqttHeart = NULL;
int mqttHeartLen = 0;
