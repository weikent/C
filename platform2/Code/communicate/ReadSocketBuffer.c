/*
 * =====================================================================================
 *
 *    Filename:  ReadSocketBuffer.c
 *
 * Description:  用于接收服务器的信息，过滤信息并发送给本地层。
 *
 *    
 *
 *     Version:  0.1
 *     Created:  Thu Apr 10 09:30:12 2014
 *
 *     Authors:  世建 魏 , weikent@ubuntu
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   世建 魏  Thu Apr 10 09:30:13 2014 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

//#include <config.h>
#include "ReadSocketBuffer.h"
#include "pthread.h"



#include "global.h"
#include "../../common/message.h"
#include "../../common/msgQ.h"
#include "tool.h"
#include "../../common/cJSON.h"



#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static unsigned long countRemainingLength();
static void sendmessageToLocal();
static int readn(int fd, void *vptr, int n);
static void filterServerData(const char *data);
static void sendmessageToLocal();
static int changeTCPStatus();


short keepAlive = 0;

static int readn(int fd, void *vptr, int n)
{
    int nleft;
    int nread;
    char *ptr;
    ptr = (char*)vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
            {
                nread = 0;
            }
            else 
            {
                return -1;
            }
        }
        else if (nread == 0)
        {
            break;
        }
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}




void *ReadSocketBufferRun(void *arg)
{
    pthread_detach(pthread_self());

    for ( ;  ;  )
    {
      startRead:
        signal( SIGPIPE, SIG_IGN );

        if (clientModelTCPSocket < 0)
        {
            usleep(500 * 1000);
            goto startRead;
        }
        else
        {

            fd_set fdsr;
            struct timeval tv;
            int maxsock = -1;

            int ret = -1;

            for ( ;  ;  )
            {
                if (clientModelTCPSocket < 0)
                {
                    usleep(500 * 1000);
                    goto startRead;
                }

                selectTimeout = false;
                errno = 0;
                FD_ZERO(&fdsr);

                FD_SET(clientModelTCPSocket, &fdsr);
                maxsock = clientModelTCPSocket;
                // timeout setting

                tv.tv_sec = 10;
                tv.tv_usec = 0;

                debug_msg("clientmodeltcpsocket = %d\n", clientModelTCPSocket);

                int selectResult = select(maxsock + 1, &fdsr, NULL, NULL, &tv);

                if (selectResult < 0) {
                    perror("---select");
                    break;
                }else if (selectResult == 0){

                    keepAlive += 10;

                    if (keepAlive > ((MQTT_KEEP_ALIVE - 10) * 3)) {
                        debug_msg("---");
                        shouldCloseSocket = true;
                        keepAlive = 0;
                    } else {
                        perror("---select timeout ");
                        selectTimeout = true;
                    }

                    break;
                }
                debug_msg("select check finished\n");
                /* 在select超时或出错的时候本该关闭socket，如果此句放到select之前的话就不会关闭socket了。 */
                shouldCloseSocket = false;

                errno = 0;
                if (FD_ISSET(clientModelTCPSocket, &fdsr))
                {
                    unsigned char byte = 255;

                    ret = readn(clientModelTCPSocket, &byte,1);

                    fprintf(stdout,"buffer = %d\n", byte);
                    fprintf(stdout,"ret = %d\n", ret);
                    if (ret <= 0)
                    {
                        fprintf(stdout,"errno = %d\n", errno);
                        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                        {
                        }
                        else
                        {
                            debug_msg("---");
                            shouldCloseSocket = true;
                            keepAlive = 0;
                        }

                        break;
                    }
                    else
                    {
                        fprintf(stdout,"Hex byte = %02X\n", byte);
                        if (byte == MESSAGE_TYPE_CONNACK) {
                            debug_msg("received connack\n");

                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();
                            debug_msg("connack remaining length = %ld\n", remainingLength);

                            if (remainingLength <= 0) {
                                break;
                            }

                            if (remainingLength != 2){
                                break;
                            }
                            unsigned char *buffer = (unsigned char*)malloc(remainingLength + 1);
                            bzero(buffer, remainingLength + 1);
                            ret = -1;
                            ret = readn(clientModelTCPSocket, buffer, remainingLength);
                            printf("buffer[0] = %02x\n", buffer[0]);
                            printf("buffer[1] = %02x\n", buffer[1]);

                            if (buffer[remainingLength - 1] == 0) {
                                g_isConnected = true;
//                                sem_post(&g_semSendHeart);
                            }
                            free(buffer);
                            break;

                        }else if (byte == MESSAGE_TYPE_SUBACK){
                            debug_msg("received suback\n");
                            g_isSubscribed = true;
                            //                          sem_post(&g_semSendHeart);
                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();

                            debug_msg("suback remaining length = %ld\n", remainingLength);

                            if (remainingLength <= 0) {
                                break;
                            }

                            unsigned char *buffer = (unsigned char*)malloc(remainingLength + 1);
                            bzero(buffer, remainingLength + 1);
                            ret = -1;
                            ret = readn(clientModelTCPSocket, buffer, remainingLength);

                            int i = 0;
                            for (i = 0; i < remainingLength; i++) {
                                fprintf(stdout,"%02X ", buffer[i]);
                            }
                            fprintf(stdout,"\n");
                            free(buffer);
                            break;
                            // start send pingreq

                        }else if (byte == MESSAGE_TYPE_PINGRESP){
                            debug_msg("received pingresp\n");
                            keepAlive = 0;

                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();

                            fprintf(stdout,"remaining length = %ld\n", remainingLength);

                            if (remainingLength <= 0) {
                                break;
                            }
                            break;
                        }else if (byte == MESSAGE_TYPE_PUBACK) {
                            debug_msg("received puback\n");
                            keepAlive = 0;
                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();

                            fprintf(stdout,"remaining length = %ld\n", remainingLength);
                
                            if (remainingLength <= 0) {
                                shouldCloseSocket = true;
                                break;
                            }

                            unsigned char *buffer = (unsigned char*)malloc(remainingLength + 1);
                            bzero(buffer, remainingLength + 1);
                            ret = -1;
                            ret = readn(clientModelTCPSocket, buffer, remainingLength);
                            
                            int i = 0;
                            for (i = 0; i < remainingLength; i++) {
                                fprintf(stdout,"%02X ", buffer[i]);
                            }
                            fprintf(stdout,"\n");

                            unsigned char packetIdentifer[3] = {0};
                            packetIdentifer[0] = buffer[1];
                            packetIdentifer[1] = buffer[0];
                            short *i_packetIdentifer = (short*)packetIdentifer;
                            fprintf(stdout,"puback packetIdentifer = %d\n", *i_packetIdentifer);

                            free(buffer);
                            break;
                        }else if (byte == 0x32 || byte == 0x3A) {
                            

                            keepAlive = 0;

                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();
                
                            fprintf(stdout,"remaining length = %ld\n", remainingLength);
                
                            if (remainingLength <= 0) {
                                shouldCloseSocket = true;
                                break;
                            }

                            unsigned char *buffer = (unsigned char*)malloc(remainingLength + 1);
                            bzero(buffer, remainingLength + 1);
                            ret = -1;

                            ret = readn(clientModelTCPSocket, buffer, remainingLength);
                
                            short *i_topicFilterLen = 0;
                            unsigned char topicFilter[3] = {0};
                            debug_msg("----");

                            if (ret <= 0) {
                                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                                {
                                }
                                else
                                {
                                    debug_msg("----");
                                    shouldCloseSocket = true;
                                    keepAlive = 0;
                                }
                            }else{
                                debug_msg("----");
                                int i = 0;
                                for (i = 0; i < remainingLength; i++) {
                                    fprintf(stdout,"%02X,", buffer[i]);
                                }
                                fprintf(stdout,"\n");

                                topicFilter[0] = buffer[1];
                                topicFilter[1] = buffer[0];
                                i_topicFilterLen = (short*)topicFilter;
                                fprintf(stdout,"topicFilter = %d\n", *i_topicFilterLen);

                            }
                            unsigned char *topicBuffer = (unsigned char*)malloc(*i_topicFilterLen + 1);
                            bzero(topicBuffer, *i_topicFilterLen + 1);
                            memcpy(topicBuffer, buffer + 2, *i_topicFilterLen);
                            /* fprintf(stdout,"topicFilter = %s\n", topicBuffer); */

                            unsigned char topicID[3] = {0};
                            topicID[0] = buffer[2 + *i_topicFilterLen + 1];
                            topicID[1] = buffer[2 + *i_topicFilterLen];
                            short *i_topicID = 0;
                            i_topicID = (short*)topicID;
                            /* fprintf(stdout,"i_topicID = %d\n", *i_topicID); */
                            /* fprintf(stdout,"topicFilter = %d\n", *i_topicFilterLen); */

                            /* fprintf(stdout,"1 = %02x\n", buffer[2 + *i_topicFilterLen]);  */
                            /* fprintf(stdout,"1 = %02x\n", buffer[2 + *i_topicFilterLen + 1]);  */
                            /* 
                               2个 length msb 和 lsb. 以及topicfilter
                            */
                
                            int lenOfActualData = remainingLength - 2 - *i_topicFilterLen - 2;
                            unsigned char *newBuffer = (unsigned char*)malloc(lenOfActualData + 1 + 8);
                            bzero(newBuffer, lenOfActualData + 1 + 8);
                            memcpy(newBuffer, buffer + 2 + *i_topicFilterLen + 2, lenOfActualData);

                            free(buffer);
                            // fprintf(stdout,"newbuffer = %s\n", newBuffer);
                            /* decry(newBuffer, lenOfActualData, key1, key2, key3); */
                            /* newBuffer[lenOfActualData - newBuffer[lenOfActualData - 1]] = '\0'; */

                            filterServerData(newBuffer);


                            free(newBuffer);

                            free(topicBuffer);
                        }else if (byte == 0x30) {  // publish qos == 0
                            keepAlive = 0;

                            unsigned long remainingLength = 0;
                            remainingLength = countRemainingLength();
                            debug_msg("remaining length = %ld\n", remainingLength);
                
                            if (remainingLength <= 0) {
                                shouldCloseSocket = true;
                                break;
                            }

                            unsigned char *buffer = (unsigned char*)malloc(remainingLength + 1);
                            bzero(buffer, remainingLength + 1);
                            ret = -1;

                            ret = readn(clientModelTCPSocket, buffer, remainingLength);
                
                            short *i_topicFilterLen = 0;
                    
                            unsigned char topicFilter[3] = {0};
                            if (ret <= 0) {
                                if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
                                {
                                }
                                else
                                {
                                    shouldCloseSocket = true;
                                    keepAlive = 0;
                                }
                            }else{
                        
                                // int i = 0;
                                // for (i = 0; i < remainingLength; i++) {
                                //     printf("%02X,", buffer[i]);
                                // }
                                // printf("\n");

                                topicFilter[0] = buffer[1];
                                topicFilter[1] = buffer[0];
                                i_topicFilterLen = (short*)topicFilter;
                                // printf("topicFilter = %d\n", *i_topicFilterLen);

                            }
                            // unsigned char *topicBuffer = (unsigned char*)malloc(*i_topicFilterLen + 1);
                            // bzero(topicBuffer, *i_topicFilterLen + 1);
                            // memcpy(topicBuffer, buffer + 2, *i_topicFilterLen);




                            // printf("topicFilter = %s\n", topicBuffer);

                            // unsigned char topicID[3] = {0};
                            // topicID[0] = buffer[2 + *i_topicFilterLen + 1];
                            // topicID[1] = buffer[2 + *i_topicFilterLen];
                            // short *i_topicID = 0;
                            // i_topicID = (short*)topicID;
                            /* printf("i_topicID = %d\n", *i_topicID); */
                            /* printf("topicFilter = %d\n", *i_topicFilterLen); */

                            /* printf("1 = %02x\n", buffer[2 + *i_topicFilterLen]);  */
                            /* printf("1 = %02x\n", buffer[2 + *i_topicFilterLen + 1]);  */
                            /* 
                               2个 length msb 和 lsb. 以及topicfilter
                            */
                            int lenOfActualData = remainingLength - 2 - *i_topicFilterLen;
                            // fprintf(stdout, "lenofactualdata = %d\n", lenOfActualData);
                            unsigned char *newBuffer = (unsigned char*)malloc(lenOfActualData + 1);
                            bzero(newBuffer, lenOfActualData + 1);
                            memcpy(newBuffer, buffer + 2 + *i_topicFilterLen, lenOfActualData);
                            free(buffer);

                            debug_msg("newbuffer = %s\n", newBuffer);

                            int i = 0;
                            for (i = 0; i < lenOfActualData; i++) {
                                printf("%02X ", newBuffer[i]);
                            }
                            printf("\n");



                            /* unsigned long cbBytes; */
                            /* unsigned char *pbData; */
                            /* char equalChar = 0; */
                            /* if (newBuffer[lenOfActualData - 1] == 0x3D && newBuffer[lenOfActualData - 2] == 0x3D) { */
                            /*     cbBytes = (lenOfActualData - 2) * 3/4; */
                            /*     equalChar = 2; */
                            /* }else if (newBuffer[lenOfActualData - 1] == 0x3D && newBuffer[lenOfActualData - 2] != 0x3D) { */
                            /*     cbBytes = (lenOfActualData - 1) * 3/4; */
                            /*     equalChar = 1; */
                            /* }else if (newBuffer[lenOfActualData - 1] != 0x3D && newBuffer[lenOfActualData - 2] != 0x3D) { */
                            /*     cbBytes = (lenOfActualData) * 3/4; */
                            /*     equalChar = 0; */
                            /* }else{ */
                            /*     cbBytes = 0; */
                            /* } */

                            /* debug_msg("cbBytes = %d\n", cbBytes); */

                            /* pbData = (unsigned char*)malloc(cbBytes + 1); */
                            /* bzero(pbData, cbBytes + 1); */

                            /* cbBytes = b64decode(newBuffer, pbData); */
                            /* debug_msg("base64 decoded = %s\n", pbData); */
                            /* debug_msg("cbBytes = %d\n", cbBytes); */

                            /* cbBytes -= equalChar; */
                            /* for (i = 0; i < cbBytes; i++) { */
                            /*     printf("%02X ", pbData[i]); */
                            /* } */
                            /* printf("\n"); */
                            /* unsigned char *newBuffer2 = malloc(cbBytes + 1 + 8); */
                            /* bzero(newBuffer2, cbBytes + 1 + 8); */
                            /* memcpy(newBuffer2, pbData, cbBytes); */
                            /* decry(newBuffer2, cbBytes, key1, key2, key3); */
                            /* newBuffer2[cbBytes - newBuffer2[cbBytes - 1]] = '\0'; */
                            /* debug_msg("newbuffer after decry = %s\n", newBuffer2); */

                            
                            // decry(newBuffer, lenOfActualData);
                            // newBuffer[lenOfActualData - newBuffer[lenOfActualData - 1]] = '\0';
                            // fprintf(stdout, "newbuffer after decry = %s\n", newBuffer);

                            /* filterServerData(newBuffer2); */
                            filterServerData(newBuffer);
                            free(newBuffer);
                            /* free(newBuffer2); */
                            /* free(pbData); */
                        }
                    }
                }
                else
                {
                    //   break;
                }
            }
        }
    }
    pthread_exit((void*)0);
}




static void filterServerData(const char *data)
{

  /* 在此处解析数据，发送消息队列 */

  struct myMsgBuf msgbuf;
  msgbuf.sender_type = MY_MSG_TYPE;
  send_message(msgID, &msgbuf, FILTER_MSG_TYPE, data);

}





static void sendmessageToLocal()
{

}




static unsigned long countRemainingLength()
{
    unsigned char byte = 255;
    int ret = -1;
    int multiplier = 1;
    unsigned long remainingLength = 0;
    do {
        byte = 255;
        ret = -1;
        ret = readn(clientModelTCPSocket, &byte,1);
        debug_msg("byte = %d\n", byte);
        remainingLength += (byte & 127) * multiplier;
        multiplier *= 128;

    } while ((byte & 128) != 0);

    return remainingLength;
}



