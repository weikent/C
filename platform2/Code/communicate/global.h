// -*- C++ -*-

/* @(#)global.h
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H 1

#ifdef __cplusplus
extern "C"
{
#endif

  #include <stdbool.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "../../common/message.h"
#include <pthread.h>
#include <semaphore.h>


#define DEBUG 1			/* switch of print debug info */

#ifdef DEBUG
#define debug_msg(args...) do                                           \
    {                                                                   \
        fprintf(stdout, "%-30s[%4d]_%-20s : ", __FILE__, __LINE__, __FUNCTION__); \
        fprintf(stdout, args);                                          \
        fprintf(stdout, "\n");                                          \
    }                                                                   \
    while(0)
#else
#define debug_msg(arg...) ((void)0)
#endif



#define MAX_DATA_SEND_TO_SERVER 3072
#define PAIR_MAX_DATA_SEND_TO_PHONE 3072

extern _Bool timeSync_ed;       /* 是否进行过时间同步 */


extern int clientModelTCPSocket;

extern _Bool shouldCloseSocket;	/* if server send close command, then, should close socket */


extern _Bool isChecking; /* connectioncheck is checking or not */

extern _Bool selectTimeout;

extern _Bool hasSendData;       /* 用来指示是否发送了数据给服务器，readsocketbuffer线程在短链接时根据此标志位来判断是否改进行select操作。 */




extern pthread_mutex_t g_pthTCPSocket; /* 一个线程锁，确保同时只有一个线程可以创建socket，并connect */


extern sem_t g_semConnectionCheck;
extern sem_t g_semNetcom;



#define LEN_OF_IPADDRESS 16	/* len of ip address */
#define LEN_OF_MAC 32 		/* Device Mac 's length */
extern char g_mac[LEN_OF_MAC];
extern char g_serverIP[LEN_OF_IPADDRESS];
  extern int g_serverPort;


#define MY_MSG_TYPE 1           // 这个程序的消息队列的type
#define FILTER_MSG_TYPE 3       // Filter程序的消息队列type
  
  extern int msgID;

#define UTF8_HEADER_LENGTH 2


#define MQTT_KEEP_ALIVE 60


/* message type */
#define MESSAGE_TYPE_CONNECT 0x10
#define MESSAGE_TYPE_CONNACK 0x20
#define MESSAGE_TYPE_PUBLISH 0x30 /* Qos = 0x01   0x00110010 */
#define MESSAGE_TYPE_PUBACK 0x40
#define MESSAGE_TYPE_SUBACK 0x90
#define MESSAGE_TYPE_SUBSCRIBE 0x82
#define MESSAGE_TYPE_PINGREQ 0xC0
#define MESSAGE_TYPE_PINGRESP 0xD0
#define MESSAGE_TYPE_DISCONNECT 0xE0


#define SUBSCRIBE_REQUEST_QOS 0x01



//#define LEN_PROTOCOL_NAME 1
#define LEN_MESSAGE_TYPE 1
#define LEN_PROTOCOL_LEVEL 1
#define LEN_CONNECT_FLAGS 1
#define LEN_KEEPLIVES 2
#define LEN_PACKET_IDENTIFIER 2
#define LEN_SUBSCRIBE_REQUEST_QOS 1


#define PROTOCOL_NAME "MQIsdp"

#define LEN_OF_TOPICFILTER 3
extern char *g_subscribeTopicFilter[];
extern short g_subscribePacketIdentifier;



#define PREFIX_PUBLISH_TOPIC_FILTER "isockets/7688Device/" /* 这个topic 发送到 topicServer， APP， Web */
#define PREFIX_PUBLISH_APP_WEB_TOPIC_FILTER "isockets/noTopic/" /* 这个topic 发送到 APP，Web */
#define LEN_OF_PREFIX_PUBLISH_TOPIC_FILTER strlen(PREFIX_PUBLISH_TOPIC_FILTER)
#define LEN_OF_PREFIX_PUBLISH_APP_WEB_TOPIC_FILTER strlen(PREFIX_PUBLISH_APP_WEB_TOPIC_FILTER)


extern char **topicList;
extern int topicListCount;





extern _Bool g_isCreated;
extern _Bool g_isConnected;
extern _Bool g_isSubscribed;
extern _Bool g_isListChanged;
//extern sem_t g_semSendHeart;




extern pthread_mutex_t g_pthDealmap;
extern pthread_mutex_t g_pthDealPubAck; /* 处理发送puback的锁 */

enum mqttMessageType {CONNECT, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK, PINGREQ, PINGRESP, DISCONNECT};



extern char messageType[2];

#define MAX_SENDINGDATA 30
#define MAX_WAITINGDATA 30      /* 保存的等待回应的数据的最大值 */



// 下面这4个struct,是用来生成一个实际可以发送到服务器端的数据包.

struct fixedHeader
{
    enum mqttMessageType messageType;  /* 生成一个发送数据包,在fixed header中必须知道是什么类型的信息 */
    char Qos;
    char DUP;
    char REATIN;
};

struct variableHeader
{
    short packetIdentifer;
    char **topicFilterArr;
    int i_topicFilterArrCount;
};

struct playLoad
{
    char *clientID;             /* 客户端ID，主要是connect时使用 */
    char *actualData;           /* 生成一个发送数据包(publish时), 在playLoad中 必须要知道实际要发送的内容  */
    int i_actualDataLen;        /* 知道数据的实际长度, 方便计算remaining length, 以及utf-8 的字符串(UTF-8 encoded strings)*/
};


struct mqttSend
{
    //enum mqtt_Message_Type message_Type;
    struct fixedHeader fixedHeader;    
    struct variableHeader variableHeader;
    struct playLoad playLoad;
};



#ifdef __cplusplus
}
#endif


#endif /* _GLOBAL_H */
