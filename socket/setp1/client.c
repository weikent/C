/*
 * =====================================================================================
 *
 *    Filename:  client.c
 *
 * Description:  对应selectServer.c的客户端
 *
 *    
 *
 *     Version:  0.1
 *     Created:  14 Jul 2014  14:16:52
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   14 Jul 2014  14:16:52 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现
#define serverIP "115.28.79.70"
#define serverPort 14567

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf ("must input serverip and serverport\n");
        return 0;
    }


    int g_iClientModelTCPSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (g_iClientModelTCPSocket < 0)
    {
        perror("socket faild");
    }

    struct sockaddr_in s_add;
            
    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    inet_pton(AF_INET, argv[1], &s_add.sin_addr);
    s_add.sin_port=htons(atoi(argv[2])); 


    struct timeval timeout = {5,0};
    socklen_t len = sizeof(timeout);
    setsockopt(g_iClientModelTCPSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);

    errno = 0;
    if(-1 == connect(g_iClientModelTCPSocket,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        if (errno == EINPROGRESS) 
        {
            perror("connect timeout");
        }
        else
        {
            perror("connect fail!");
        }
    }
    else
    {
        perror("Connect Success!");



        printf ("g_iClientModelTCPSocket = %d\n", g_iClientModelTCPSocket);

        struct timeval timeout = {5,0};
        socklen_t len = sizeof(timeout);
        setsockopt(g_iClientModelTCPSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);

      reWrite:
        if (write(g_iClientModelTCPSocket, "hello server", 13) == -1)
        {
            /* EPIPE:Broken pipe.
               原因是。在write时，socket已经关闭了。不论是服务器关闭或程序中其他线程关闭都一样。
               但此原因不应该导致重新进行网络检查。不是网络不通造成的发送失败*/
            if (errno == EPIPE) 
            {
                printf ("g_iClientModelTCPSocket = %d\n", g_iClientModelTCPSocket);
            }
        }
        else
        {
            printf ("Success\n");
//            sleep(2);
//            goto reWrite;
        }

        unsigned char bufread[100] = {0};
        read(g_iClientModelTCPSocket, bufread, 20);
        printf ("%s\n", bufread);

        goto reWrite;
        close(g_iClientModelTCPSocket);

    }
    return 0;
}
