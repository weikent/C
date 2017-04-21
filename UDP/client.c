/*
 * =====================================================================================
 *
 *    Filename:  client.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  21 Apr 2017  16:00:23
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   21 Apr 2017  16:00:23 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件


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


#include<stdlib.h>  
#include<stdio.h>  
#include<string.h>  
#include<sys/types.h>  
#include<netinet/in.h>  
#include<netdb.h>  
#include<sys/socket.h>  
#include<sys/wait.h>  
#include<arpa/inet.h>  
int main(){  
    int sockListen;  
    if((sockListen = socket(AF_INET, SOCK_DGRAM, 0)) == -1){  
        printf("socket fail\n");  
        return -1;  
    }  
    int set = 1;  
    setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));  
    struct sockaddr_in recvAddr;  
    memset(&recvAddr, 0, sizeof(struct sockaddr_in));  
    recvAddr.sin_family = AF_INET;  
    recvAddr.sin_port = htons(4001);  
    recvAddr.sin_addr.s_addr = INADDR_ANY;  
    // 必须绑定，否则无法监听  
    if(bind(sockListen, (struct sockaddr *)&recvAddr, sizeof(struct sockaddr)) == -1){  
        printf("bind fail\n");  
        return -1;  
    }  
    int recvbytes;  
    char recvbuf[128];  
    int addrLen = sizeof(struct sockaddr_in);  
    if((recvbytes = recvfrom(sockListen, recvbuf, 128, 0,  
        (struct sockaddr *)&recvAddr, &addrLen)) != -1){  
        recvbuf[recvbytes] = '\0';  
        printf("receive a broadCast messgse:%s\n", recvbuf);  
    }else{  
        printf("recvfrom fail\n");  
    }  
    close(sockListen);  
    return 0;  
}  
