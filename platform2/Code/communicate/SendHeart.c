/*
 * =====================================================================================
 *
 *    Filename:  SendHeart.c
 *
 * Description: send mqtt information.
 *              included connect, subscribe, pingresq
 *
 *
 *     Version:  0.1
 *     Created:  Wed Apr  9 16:32:58 2014
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian  Wed Apr  9 16:32:58 2014 , create orignal file
 * @for mqtt weishijian 2015/07/08 09:15:00    for mqtt
 * @for mqtt weishijian 2015/07/28 10:57:58    for mqtt, add topic server. so need to change thr process of the connect.
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */


//#include <config.h>
#include "SendHeart.h"


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>


#include "global.h"
#include "../../common/message.h"
#include "SendSocket.h"
#include "packMessage.h"
#include "tool.h"



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: SendHeartRun
 *  Description: 用于发送mqtt的connect。subscribe。pingreq
 *   Parameters: 
 /// @param arg 
 * 
 * Return Value: 
 *      Created: Mon Sep  7 16:07:28 2015 byweishijian
 *     Revision: 
 * =======================================================================================
 * @0.1   weishijian  Mon Sep  7 16:07:28 2015 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

void *SendHeartRun(void *arg)
{
  pthread_detach(pthread_self());


  char intervalOfSendPingREQ = 0;
  char intervalOfSendGetTime = 0;
  char intervalOfSendHeart = 0;
  char intervalKeepAlive = MQTT_KEEP_ALIVE / 10;


  _Bool needSendPublish = false;

  while(1)
    {
      debug_msg("5s");
      sleep(5);

      if (g_isCreated) {
        debug_msg("have created socket and connect to server\n");

        //                debug_msg("before wait semsendheart\n" );
        //                sem_wait(&g_semSendHeart);

        //                debug_msg("after wait semsendheart\n ");
        if (g_isConnected) { /* connected to mqtt server */

          debug_msg("g_isSubscribed = %d\n", g_isSubscribed);
          if (g_isSubscribed) {

            intervalOfSendPingREQ ++;
            intervalOfSendGetTime ++;


            if (intervalOfSendPingREQ > intervalKeepAlive) {

              unsigned char pingREQ[2] = {0};
              pingREQ[0] = 0xC0;
              pingREQ[1] = 0x00;
              sendSocket(pingREQ, 2, "mqtt pingreq");

              intervalOfSendPingREQ = 0;
            }
            /* 120 * 5 = 600s = 1hour, so get current time from topicServer every 1 hour */
            if (intervalOfSendGetTime > 120) {
              intervalOfSendGetTime = 0;
              /* TODO: get time form topicServer */
            }
          } else {
            debug_msg("---");
            // is not received suback from server.

            /* so should send mqtt subscribe */
            struct mqttSend *mqtt = malloc(sizeof(struct mqttSend));

            /* set message type */
            mqtt->fixedHeader.messageType = SUBSCRIBE;

            /* set topic filter */
            printf("g_mac len = %d\n", strlen(g_mac));
            int count = LEN_OF_TOPICFILTER;
            int i = 0;

            mqtt->variableHeader.topicFilterArr = (char**)malloc(count * sizeof(char*));

            /* set topic filter length */
            mqtt->variableHeader.i_topicFilterArrCount = count;

            for (i = 0; i < count; i++) {
              unsigned char *topicFilter = malloc(strlen(g_subscribeTopicFilter[i]) + strlen(g_mac) + 1);
              bzero(topicFilter, strlen(g_subscribeTopicFilter[i]) + strlen(g_mac) + 1);

              memcpy(topicFilter, g_subscribeTopicFilter[i], strlen(g_subscribeTopicFilter[i]));

              memcpy(topicFilter + strlen(g_subscribeTopicFilter[i]), g_mac, strlen(g_mac));

              printf("topicFilter len = %lu\n", strlen(topicFilter));

              *(mqtt->variableHeader.topicFilterArr + i) = malloc(strlen(topicFilter) + 1);

              bzero(*(mqtt->variableHeader.topicFilterArr + i), strlen(topicFilter) + 1);

              memcpy(*(mqtt->variableHeader.topicFilterArr + i), topicFilter, strlen(topicFilter));

              free(topicFilter);
            }

            unsigned char *temp = NULL;
            int len = packMessage(mqtt, &temp);

            for (i = 0; i < count; i++) {
              free(*(mqtt->variableHeader.topicFilterArr + i));
            }

            free(mqtt->variableHeader.topicFilterArr);
            free(mqtt);

            printf("publishlen = %d\n", len);
            for (i = 0; i < len; i++) {
              printf("%02X,", temp[i]);
            }

            printf("\n" );

            int ret = sendSocket(temp, len, "mqtt subscribe");
            if (ret == -2) {
              //                            sem_post(&g_semSendHeart); /* for re-send this message */
            } else {
            }

            free(temp);
          }
        } else {
          debug_msg("should send mqtt connect\n");
          // is not received connack from server.

          /* so should send mqtt connect */
          /* send connect message */
          struct mqttSend * mqtt = malloc(sizeof(struct mqttSend));

          mqtt->fixedHeader.messageType = CONNECT;

          mqtt->playLoad.clientID = malloc(LEN_OF_MAC + 1);
          bzero(mqtt->playLoad.clientID, LEN_OF_MAC + 1);
          memcpy(mqtt->playLoad.clientID, g_mac, LEN_OF_MAC);

          unsigned char *temp = NULL;

          int len = packMessage(mqtt, &temp);

          free(mqtt->playLoad.clientID);
          free(mqtt);

          int i = 0;

          printf("len = %d\n", len);
          for (i = 0; i < len; i++) {
            printf("%02X,", temp[i]);
          }
          printf("\n" );

          int ret = sendSocket(temp, len, "mqtt connect"); 
          if (ret == -2) {
            //                        sem_post(&g_semSendHeart); /* to re-send this message */
          } else {

          }
          free(temp);
        }
      } else {
        debug_msg("not create socket, should create it\n");
        // should create socket, and connect to server.

        /* if is reConnect network, then continue */
        if (isChecking) {
          debug_msg("--");
          continue;
        } else {
          sem_post(&g_semConnectionCheck);
        } /* end isChecking else */
      }  /* end g_isCreated else */
    }
  pthread_exit((void*)0);
}
