/*
 * =====================================================================================
 *
 *    Filename:  server2.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  21 Apr 2017  16:07:52
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   21 Apr 2017  16:07:52 , create orignal file
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

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>

#define BUFFER_SIZE 1024


int main()
{
  char *ip="255.255.255.255";    //广播地址

  char *port1 = "4001";         //广播的端口号

  int on=1;

  int port = atoi(port1);
  int ret = 0;
  int udpfd;
  char buf[BUFFER_SIZE]={
    "hello,world"
  };
  struct sockaddr_in address;


  bzero(&address,sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET,ip,&address.sin_addr);
  address.sin_port = htons(port);
  udpfd = socket(PF_INET,SOCK_DGRAM,0);
  assert(udpfd >= 0);

  //将udpfd的属性设置为广播
  if((setsockopt(udpfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on))) == -1)

    {
      printf("error\n");
    }

  sendto(udpfd,buf,BUFFER_SIZE-1,0,(struct sockaddr *)&address,sizeof(address));

  return 0;
}
