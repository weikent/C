/*
 * =====================================================================================
 *
 *    Filename:  socket_http.c
 *
 * Description:  使用socket来发送http post
 *
 *    
 *
 *     Version:  0.1
 *     Created:  23 Jan 2015  13:34:20
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   23 Jan 2015  13:34:20 , create orignal file
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


/* 利用C语言实现POST数据包如此简单【模拟网页提交表单】 */
/* 来源：本站原创  C/C++, 原创 超过8,543 views围观 1条评论0 */


//*****************************************//
//*********利用C实现自动发包***************//
//*********作者：马   犇******************//
//*********时间：2011-6-25*****************//
//*****************************************//

//#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
//#include "winsock2.h"
//#pragma comment(lib,"ws2_32.lib")
int main(int argc, char* argv[])
{

  SOCKET hsocket;
  SOCKADDR_IN saServer;
  WSADATA     wsadata;
  LPHOSTENT lphostent;
  int     nRet;
  char*   host_name="www.********.com";
  char*   reqHead="POST ********* HTTP/1.1\r\n"   //此为要发送的数据包
        "Accept: */*\r\n"
        "Referer: http://www.*********\r\n"
        "Accept-Language: zh-CN\r\n" 
        "User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0; BOIE9;ZHCN; twchrome)\r\n" 
        "Content-Type: application/x-www-form-urlencoded\r\n" 
        "Accept-Encoding: gzip, deflate\r\n"
        "Host: www.*******.com\r\n" 
        "Content-Length: 375\r\n"
        "Connection: Keep-Alive\r\n"
        "Cache-Control: no-cache\r\n"
        "Cookie: ************** \r\n\r\n"
        "user=test&pass=123";
        
 // 初始化套接字 
    if(WSAStartup(MAKEWORD(2,2),&wsadata))
        printf("初始化SOCKET出错！");
    lphostent=gethostbyname(host_name);     
    if(lphostent==NULL)     
        printf("lphostent为空！");  
    hsocket   =   socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);     
    saServer.sin_family = AF_INET;     
    saServer.sin_port = htons(80);     
    saServer.sin_addr = *((LPIN_ADDR)*lphostent->h_addr_list);  
    // 利用SOCKET连接  
    nRet = connect(hsocket,(LPSOCKADDR)&saServer,sizeof(SOCKADDR_IN));     
    if(nRet == SOCKET_ERROR)     
    {
        printf("建立连接时出错！");     
        closesocket(hsocket);
        return 0;
    }
    // 利用SOCKET发送  

    nRet = send(hsocket,req,strlen(req),0);  
    if(nRet==SOCKET_ERROR)  
    {  
        printf("发送数据包时出错！");  
        closesocket(hsocket);  
    }  
    char   Dest[3000];     
    nRet=1;     
    while(nRet>0)     
    {  
        // 接收返回数据包  
        nRet=recv(hsocket,(LPSTR)Dest,sizeof(Dest),0);     
        if(nRet>0)
            Dest[nRet]=0;
        else     
            Dest[0]=0;  
        // 显示返回数据包的大小、内容 
        printf("\nReceived   bytes:%d\n",nRet);     
        printf("Result:\n%s",Dest);     
    }
 }
    return 0; 
}