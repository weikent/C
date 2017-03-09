/*
 * =====================================================================================
 *
 *    Filename:  selectServer.c
 *
 * Description:  此文件用来联系 select 函数
 *
 *    
 *
 *     Version:  0.1
 *     Created:  14 Jul 2014  13:46:18
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   14 Jul 2014  13:46:18 , create orignal file
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

#define BACKLOG 100
#define BUF_SIZE 1024
#define MYPORT 1883


int main(int argc, char *argv[])
{

    int fd_A[BACKLOG];    // accepted connection fd
    int conn_amount;    // current connection amount


    int sock_fd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in server_addr;    // server address information
    struct sockaddr_in client_addr; // connector's address information
    socklen_t sin_size;
    int yes = 1;
    char buf[BUF_SIZE];
    int ret;
    int i;


    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
    }

    printf ("sock_fd = %d\n", sock_fd);

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    {
        perror("setsockopt");
    }

    server_addr.sin_family = AF_INET;         // host byte order
    server_addr.sin_port = htons(MYPORT);     // short, network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
    }

    if (listen(sock_fd, 5) == -1) 
    {
        perror("listen");
    }

    printf("listen port %d\n", MYPORT);

    memset(fd_A, 0, sizeof(fd_A));
    
    fd_set fdsr;
    int maxsock;
    struct timeval tv;

    conn_amount = 0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;


    for ( ;  ;  )
    {

        errno = 0;
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);


        tv.tv_sec = 30;
        tv.tv_usec = 0;

        for (i = 0; i < BACKLOG; i++) 
        {
            if (fd_A[i] <= 0)
            {
                fd_A[i] = 0;
            }
            
//            printf ("fd_A[%d] = %d\n",i,fd_A[i]);

            if (fd_A[i] != 0) 
            {
                FD_SET(fd_A[i], &fdsr);
            }
        }


        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);


        if (ret < 0) 
        {
            perror("select");
            continue;
        }
        else if (ret == 0) 
        {
            printf("timeout\n");
            continue;
        }


// check every fd in the set

        for (i = 0; i < conn_amount; i++) 
        {
            if (FD_ISSET(fd_A[i], &fdsr)) 
            {
                memset(buf, 0 , BUF_SIZE);

                errno = 0;

                ret = recv(fd_A[i], buf, BUF_SIZE, 0);

                if (ret <= 0)    // client close
                {
                    conn_amount --;
                    printf("client[%d] close\n", i);

                    close(fd_A[i]);
                    FD_CLR(fd_A[i], &fdsr);
                    fd_A[i] = 0;
                }
                else
                {
                    printf ("buffer = %s\n",buf);
                }


                send(fd_A[i], "~000588888", 10, 0);
            }

        }


// check whether a new connection comes
        if (FD_ISSET(sock_fd, &fdsr)) 
        {
            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);

            if (new_fd <= 0) 
            {
                perror("accept");
                continue;
            }

            // add to fd queue
            if (conn_amount < BACKLOG) 
            {
/* fd_A[conn_amount++] = new_fd; */
                for (i = 0; i < BACKLOG; i++)
                {
                    if (fd_A[i] == 0)
                    {
                        fd_A[i] = new_fd;
                        break;
                    }
                }


                printf("new connection client[%d] %s:%d\n", conn_amount,
                       inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                conn_amount ++;

                printf ("new_fd = %d\n", new_fd);

                printf ("maxsock = %d\n", maxsock);
                if (new_fd > maxsock)
                {
                    maxsock = new_fd;
                }
            }
            else
            {
                printf("max connections arrive, exit\n");

                send(new_fd, "bye", 4, 0);
                close(new_fd);
            }
        }
    }


    for (i = 0; i < BACKLOG; i++) 
    {
        if (fd_A[i] != 0) 
        {
            printf ("close %d\n", fd_A[i]);
            close(fd_A[i]);
        }
    }

    close(sock_fd);

    return 0;
}