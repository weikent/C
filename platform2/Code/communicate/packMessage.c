/*
 * =====================================================================================
 *
 *    Filename:  packMessage.c
 *
 * Description:  用于生成各种MQTT的包的文件.
 *
 *    
 *
 *     Version:  0.1
 *     Created:  23 Jun 2015  10:45:42
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   23 Jun 2015  10:45:42 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 
#include "packMessage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: ReverseArray
 *  Description: 反序一个字节数组,一般在机器中,都是按照小段来存储的, 这个函数转换为大端自序
 为了适应utf-8的表示方式
 *   Parameters: 
/// @param cValue 
/// @param nLen 
/// @param cReturnValue 
* 
* Return Value: 
*      Created: Wed Jun 24 15:04:51 2015 byweishijian
*     Revision: 
* =======================================================================================
* @0.1   weishijian  Wed Jun 24 15:04:51 2015 , create orignal file
* =======================================================================================
* Copyright (c), DingQing Technology, Ltd.
* =======================================================================================
*/

int ReverseArray(const unsigned char *cValue, short nLen, unsigned char *cReturnValue)
{
    int i = 0;
    for (i = 0; i < nLen; i++)
    {
        cReturnValue[i] = cValue[nLen - i - 1];
    }
    return 0;
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: makeutf8String
 *  Description: 返回一个utf8存储方式的字符串,  使用大端方式表示字符串长度(长度用2个字节表示). 之后跟着实际的长度.
 *   Parameters: 
/// @param originalString
/// @param destinationString
*      Created: Wed Jun 24 15:15:38 2015 byweishijian
*     Revision: 
* =======================================================================================
* @0.1   weishijian  Wed Jun 24 15:15:38 2015 , create orignal file
* =======================================================================================
* Copyright (c), DingQing Technology, Ltd.
* =======================================================================================
*/

void makeutf8String(unsigned char *originalString, unsigned char **destinationString )
{

#ifdef DEBUG
//    printf("originalstring = %s\n", originalString);
#endif // DEBUG

    short len = strlen((char*)originalString);
#ifdef DEBUG
//    printf("originalstring len = %d\n", len);
#endif // DEBUG

    short newLen = len;

    unsigned char *tempString = malloc(len + UTF8_HEADER_LENGTH + 1);
    bzero(tempString, len + UTF8_HEADER_LENGTH);

    unsigned char cLen[UTF8_HEADER_LENGTH + 1] = {0};
    ReverseArray((unsigned char *)&newLen, UTF8_HEADER_LENGTH, cLen);
    memcpy(tempString, cLen, UTF8_HEADER_LENGTH);

    memcpy(tempString + UTF8_HEADER_LENGTH, originalString, len);

#ifdef DEBUG
    /* int i = 0; */
    /* for (i = 0; i < newLen; i++) { */
    /*     printf("%02X,", tempString[i]); */
    /* } */
#endif // DEBUG
    *destinationString = tempString;

}

int countRemainingLength(int length, unsigned char **str)
{
    int len = length;
    unsigned char temp[5] = {0};
    int i = 0;
    do
    {
        int digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit = digit | 0x80;

        } else {
            
        }         
        temp[i] = digit;
        i++;
        
    } while (len > 0);
    
    unsigned char *resultStr = malloc(i + 1);
    bzero(resultStr, i + 1);
    memcpy(resultStr, temp, i);
    *str = resultStr;


    return i;
}

int packMessage(struct mqttSend *mqttSend, unsigned char **packedMessage)
{

    char messageType[2] = {0};

    // need to count remaining length

    if (mqttSend->fixedHeader.messageType == CONNECT) {

        messageType[0] = MESSAGE_TYPE_CONNECT;
        unsigned char cProtocolName[] = PROTOCOL_NAME;
        unsigned char *cUtf8ProtocolName = NULL;

        makeutf8String(cProtocolName, &cUtf8ProtocolName);

        /* int i = 0; */
        /* for (i = 0; i < 6; i++) { */
        /*     printf("%02X,", cUtf8ProtocolName[i]); */
        /* } */
        /* printf("\n" ); */

        /* protocol level */
        unsigned char cProtocolLevel[LEN_PROTOCOL_LEVEL + 1] = {0};
        cProtocolLevel[0] = 0x03;
        
        /* connect flags */
        unsigned char cConnectFlags[LEN_CONNECT_FLAGS + 1] = {0};
        cConnectFlags[0] = 0x02;
        
        /* keeplives */
        unsigned char cKeepLive[LEN_KEEPLIVES + 1] = {0};
        short i_keepLive = MQTT_KEEP_ALIVE;
        ReverseArray((unsigned char*)&i_keepLive, LEN_KEEPLIVES, cKeepLive);


        /* client identifier */

        unsigned char *cUtf8ClientIdentifier = NULL;
        makeutf8String((unsigned char*)mqttSend->playLoad.clientID, &cUtf8ClientIdentifier);

        /* remaining length */
        int remainingLength = 0;
        remainingLength = strlen(PROTOCOL_NAME) + 2 + LEN_PROTOCOL_LEVEL + LEN_CONNECT_FLAGS + LEN_KEEPLIVES + strlen(mqttSend->playLoad.clientID) + 2;
        
        unsigned char *tempstr = NULL;
        int i_byteRemainingLength = countRemainingLength(remainingLength, &tempstr);
        

        unsigned char *result = malloc(remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength);
        memcpy(result, messageType, LEN_MESSAGE_TYPE);
        memcpy(result + LEN_MESSAGE_TYPE, tempstr, i_byteRemainingLength);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength, cUtf8ProtocolName, strlen(PROTOCOL_NAME) + 2);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(PROTOCOL_NAME) + 2, cProtocolLevel, LEN_PROTOCOL_LEVEL);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(PROTOCOL_NAME) + 2 + LEN_PROTOCOL_LEVEL, cConnectFlags, LEN_CONNECT_FLAGS);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(PROTOCOL_NAME) + 2 + LEN_PROTOCOL_LEVEL + LEN_CONNECT_FLAGS, cKeepLive, LEN_KEEPLIVES);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(PROTOCOL_NAME) + 2 + LEN_PROTOCOL_LEVEL + LEN_CONNECT_FLAGS + LEN_KEEPLIVES, cUtf8ClientIdentifier, strlen(mqttSend->playLoad.clientID) + 2);
        *packedMessage = result;

        free(tempstr);
        free(cUtf8ProtocolName);
        free(cUtf8ClientIdentifier);
        return i_byteRemainingLength + LEN_MESSAGE_TYPE + remainingLength;


        
        
    } else if (mqttSend->fixedHeader.messageType == PUBLISH) {


        messageType[0] = MESSAGE_TYPE_PUBLISH;
        
        /* make the topic filter */
        unsigned char *cUtfTopicFilter = NULL;
        makeutf8String(*(mqttSend->variableHeader.topicFilterArr), &cUtfTopicFilter);

        /* packet identifier */
        // short i_packetIdentifier = mqttSend->variableHeader.packetIdentifer;
        // debug_msg;
        // printf("i_packetIdentifier = %d\n", mqttSend->variableHeader.packetIdentifer);
        // unsigned char c_packetIdentifier[LEN_PACKET_IDENTIFIER + 1] = {0};
        // ReverseArray((unsigned char*)&i_packetIdentifier, LEN_PACKET_IDENTIFIER, c_packetIdentifier);
        // printf("00 = %02x\n", c_packetIdentifier[0]);
        // printf("01 = %02x\n", c_packetIdentifier[1]);

        /* remaining length */
        int remainingLength = 0;
        remainingLength = mqttSend->playLoad.i_actualDataLen /* payload's length */
            + strlen(*(mqttSend->variableHeader.topicFilterArr)) + 2 /* topic filter's length */
//            + 2                 /* packet identifier length */
            ;

        unsigned char *byteRemainingLength = NULL;
        int i_byteRemainingLength = countRemainingLength(remainingLength, &byteRemainingLength);


        /* resutl */
        unsigned char *result = malloc(remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength); 
        bzero(result, remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength);
        memcpy(result, messageType, LEN_MESSAGE_TYPE);
        memcpy(result + LEN_MESSAGE_TYPE, byteRemainingLength, i_byteRemainingLength);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength, cUtfTopicFilter, strlen(*(mqttSend->variableHeader.topicFilterArr)) + 2);
//        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(*(mqttSend->variableHeader.topicFilterArr)) + 2, c_packetIdentifier, LEN_PACKET_IDENTIFIER);
//        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(*(mqttSend->variableHeader.topicFilterArr)) + 2 + LEN_PACKET_IDENTIFIER, mqttSend->playLoad.actualData, mqttSend->playLoad.i_actualDataLen);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + strlen(*(mqttSend->variableHeader.topicFilterArr)) + 2, mqttSend->playLoad.actualData, mqttSend->playLoad.i_actualDataLen);

        free(byteRemainingLength);
        free(cUtfTopicFilter);
        
        *packedMessage = result;

        return remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength;
        
    } else if (mqttSend->fixedHeader.messageType == SUBSCRIBE) {
        messageType[0] = MESSAGE_TYPE_SUBSCRIBE;
        
        /* packet identifier */
        short i_packetIdentifier = 1;
        unsigned char c_packetIdentifier[LEN_PACKET_IDENTIFIER + 1] = {0};
        ReverseArray((unsigned char*)&i_packetIdentifier, LEN_PACKET_IDENTIFIER, c_packetIdentifier);


        /* subscribe request Qos */
        unsigned char c_subscribeRequestQos[LEN_SUBSCRIBE_REQUEST_QOS + 1] = {0};
        c_subscribeRequestQos[0] = SUBSCRIBE_REQUEST_QOS;

        
        /* make the topic filter */

        int count = mqttSend->variableHeader.i_topicFilterArrCount;
        int i = 0;
        /* 先计算出topicFilter的总长度 */
        int topicFilterTotalLen = 0;
        for (i = 0; i < count; i++) {
            topicFilterTotalLen += (strlen(*(mqttSend->variableHeader.topicFilterArr + i)) + 3);
        }


        unsigned char *finalTopicFilter = (unsigned char*)malloc(topicFilterTotalLen + 1);
        bzero(finalTopicFilter, topicFilterTotalLen + 1);

        int offset = 0;
        for (i = 0; i < count; i++) {
            int len = strlen(*(mqttSend->variableHeader.topicFilterArr + i));
            printf("len = %d\n", len);
            unsigned char *cTempTopicFilter = NULL;
            printf("topic = %s\n", *(mqttSend->variableHeader.topicFilterArr + i));
            makeutf8String((unsigned char*)(*(mqttSend->variableHeader.topicFilterArr + i)), &cTempTopicFilter);
            memcpy(finalTopicFilter + offset, cTempTopicFilter, len + 2);
            memcpy(finalTopicFilter + offset + len + 2, c_subscribeRequestQos, LEN_SUBSCRIBE_REQUEST_QOS);
            free(cTempTopicFilter);
            offset += (len + 3);
        }

        

        /* remaining length */
        int remainingLength = 0;
        
        remainingLength = LEN_PACKET_IDENTIFIER
            + topicFilterTotalLen
            ;
        printf("remaininglength = %d\n", remainingLength);
        unsigned char *byteRemainingLength = NULL;
        int i_byteRemainingLength = countRemainingLength(remainingLength, &byteRemainingLength);


        /* result */
        unsigned char *result = (unsigned char*)malloc(remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength); 
        bzero(result, remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength);
        memcpy(result, messageType, LEN_MESSAGE_TYPE);
        memcpy(result + LEN_MESSAGE_TYPE, byteRemainingLength, i_byteRemainingLength);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength, c_packetIdentifier, LEN_PACKET_IDENTIFIER);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength + LEN_PACKET_IDENTIFIER, finalTopicFilter, topicFilterTotalLen);


        free(byteRemainingLength);
        free(finalTopicFilter);
        
        *packedMessage = result;

        return remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength;
        
    } else if (mqttSend->fixedHeader.messageType == PUBACK) {
        
        messageType[0] = MESSAGE_TYPE_PUBACK;

        /* remaining length */
        int remainingLength = 0;
        remainingLength = 2;
        
        unsigned char *tempstr = NULL;
        int i_byteRemainingLength = countRemainingLength(remainingLength, &tempstr);
        
        /* packet identifier */
        short i_packetIdentifier = mqttSend->variableHeader.packetIdentifer;
        debug_msg("i_packetIdentifier = %d\n", mqttSend->variableHeader.packetIdentifer);
        unsigned char c_packetIdentifier[LEN_PACKET_IDENTIFIER + 1] = {0};
        ReverseArray((unsigned char*)&i_packetIdentifier, LEN_PACKET_IDENTIFIER, c_packetIdentifier);
        printf("00 = %02x\n", c_packetIdentifier[0]);
        printf("01 = %02x\n", c_packetIdentifier[1]);


        unsigned char *result = malloc(remainingLength + LEN_MESSAGE_TYPE + i_byteRemainingLength);
        memcpy(result, messageType, LEN_MESSAGE_TYPE);
        memcpy(result + LEN_MESSAGE_TYPE, tempstr, i_byteRemainingLength);
        memcpy(result + LEN_MESSAGE_TYPE + i_byteRemainingLength, c_packetIdentifier, LEN_PACKET_IDENTIFIER);
        *packedMessage = result;

        free(tempstr);

        return i_byteRemainingLength + LEN_MESSAGE_TYPE + remainingLength;

    } else if (mqttSend->fixedHeader.messageType == DISCONNECT) {
        unsigned char *result = malloc(3); 
        bzero(result, 3);
        result[0] = MESSAGE_TYPE_DISCONNECT;
        result[1] = 0x00;
        *packedMessage = result;
        return 2;
    } else if (mqttSend->fixedHeader.messageType == PINGREQ) {
        unsigned char *result = malloc(3); 
        bzero(result, 3);
        result[0] = MESSAGE_TYPE_PINGREQ;
        result[1] = 0x00;
        *packedMessage = result;
        return 2;
    }


    return 0;
}

/* char *connect() */
/* { */
/*     struct mqttSend *mqtt2 = malloc(sizeof(struct mqttSend)); */

/*     /\* set message type *\/ */
/*     mqtt2->fixedHeader.messageType = SUBSCRIBE; */

/*     /\* set topic filter *\/ */
/*     unsigned char topicFilter[] = "isockets/test"; */

/*     printf("topicFilter len = %d\n", sizeof(topicFilter)); */
/*     mqtt2->variableHeader.topicFilter = malloc(sizeof(topicFilter)); */
/*     bzero(mqtt2->variableHeader.topicFilter, sizeof(topicFilter)); */
/*     memcpy(mqtt2->variableHeader.topicFilter, topicFilter, sizeof(topicFilter) - 1); */

/*     /\* set topic filter length *\/ */
/*     mqtt2->variableHeader.i_topicFilterLen = sizeof(topicFilter) - 1; */


    
/*     unsigned char *temp2 = NULL; */
/*     int publishLen = packMessage(mqtt2, &temp2); */

    
/*     int i = 0; */
/*     printf("publishlen = %d\n", publishLen); */
/*     for (i = 0; i < publishLen; i++) { */
/*         printf("%02X,", temp2[i]); */
/*     } */

/*     printf("\n" ); */


/*     printf("%s\n", "----------end test subscribe pack----------"); */





/*     return temp2; */

/* } */



/* int main(int argc, char *argv[]) */
/* { */
/*     /\* test subscribe message pack *\/ */

/*     printf("----------start test subscribe message pack ----------\n"); */

/*     struct mqttSend *mqtt2 = malloc(sizeof(struct mqttSend)); */

/*     /\* set message type *\/ */
/*     mqtt2->fixedHeader.messageType = SUBSCRIBE; */

/*     /\* set topic filter *\/ */
/*     unsigned char topicFilter[] = "isockets/test"; */

/*     printf("topicFilter len = %d\n", sizeof(topicFilter)); */
/*     mqtt2->variableHeader.topicFilter = malloc(sizeof(topicFilter)); */
/*     bzero(mqtt2->variableHeader.topicFilter, sizeof(topicFilter)); */
/*     memcpy(mqtt2->variableHeader.topicFilter, topicFilter, sizeof(topicFilter) - 1); */

/*     /\* set topic filter length *\/ */
/*     mqtt2->variableHeader.i_topicFilterLen = sizeof(topicFilter) - 1; */


    
/*     unsigned char *temp2 = NULL; */
/*     int publishLen = packMessage(mqtt2, &temp2); */

    
/*     int i = 0; */
/*     printf("publishlen = %d\n", publishLen); */
/*     for (i = 0; i < publishLen; i++) { */
/*         printf("%02X,", temp2[i]); */
/*     } */

/*     printf("\n" ); */


/*     printf("%s\n", "----------end test subscribe pack----------"); */

/*     return 0; */
/* } */
