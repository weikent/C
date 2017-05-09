/* 
 *     Created:  06 12月 2013 08:45:29
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include "SendSocket.h"


#include "global.h"

#include "tool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>        /* for keepalive */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>




#include "packMessage.h"


#define RT_RDM_CMD_READ			0x6B03
#define RT_RDM_CMD_SET_BASE		0x6B0D


// static int readn(int fd, void *vptr, int n)
// {
//     int nleft;
//     int nread;
//     char *ptr;
//     ptr = (char*)vptr;
//     nleft = n;

//     while (nleft > 0)
//     {
//         if ((nread = read(fd, ptr, nleft)) < 0)
//         {
//             if (errno == EINTR)
//             {
//                 nread = 0;
//             }
//             else 
//             {
//                 return -1;
//             }
//         }
//         else if (nread == 0)
//         {
//             break;
//         }
//         nleft -= nread;
//         ptr += nread;
//     }
//     return (n - nleft);
// }




static int get_linkstatu()
{
    int fd;
    int offset;
    int ret=0;
    fd = open("/dev/rdm0", O_RDONLY);
    offset=0xB0110000;
    ioctl(fd, RT_RDM_CMD_SET_BASE, &offset);
    offset=0x80;
    ioctl(fd, RT_RDM_CMD_READ, &offset);
    if(offset&(3<<28))
    {
        ret=1;
    }
    close(fd);
    return ret;
}

/* static void startCheckConnection() */
/* { */
/*     if (isChecking) */
/*     { */
/* #ifdef RUN         */
/*         printf ("connectionCheck is Checking, return\n"); */
/* #endif */
/*         return; */
/*     } */

/*     int temp; */
/*     pthread_t connectionCheckRun; */

/*     if((temp = pthread_create(&connectionCheckRun, NULL, ConnectionCheckRun, NULL)) != 0) */
/*     { */
/* #ifdef RUN         */
/*         printf("create thread for ConnectionCheckRun failed !\n"); */
/* #endif */
/*     } */
/*     else */
/*     { */
/* #ifdef RUN */
/*         printf("create thread for ConnectionCheckRun successed !\n"); */
/* #endif */
/*     } */
/* } */

/* static void startReadSocket() */
/* { */
/*     if (isRunning) */
/*     { */
/*         printf ("socketRead is Running, return\n"); */
/*         return; */
/*     } */
/*     int temp; */
/*     pthread_t readSocketBuffer; */

/*     if((temp = pthread_create(&readSocketBuffer, NULL, ReadSocketBufferRun, NULL)) != 0) */
/*     { */
/* #ifdef TCPCLIENT_RUN */
/*         printf("create thread for ReadSocketBufferRun failed !\n"); */
/* #endif */
/*     } */
/*     else */
/*     { */
/* #ifdef TCPCLIENT_RUN */
/*         printf("create thread for ReadSocketBufferRun successed !\n"); */
/* #endif */
/*     } */
/* } */




int setKeepAlive()
{
    int keepalive = 1; // 开启keepalive属性
    int keepidle = 7; // 如该连接在60秒内没有任何数据往来,则进行探测
    int keepinterval = 1; // 探测时发包的时间间隔为5 秒
    int keepcount = 3; // 探测尝试的次数。如果第1次探测包就收到响应了,则后2次的不再发。
    setsockopt(clientModelTCPSocket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    setsockopt(clientModelTCPSocket, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
    setsockopt(clientModelTCPSocket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
    setsockopt(clientModelTCPSocket, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));
    return 0;
}

/**************************/
/* -2: socket is used     */
/* -1: write/connect 失败 */
/* 0 : 正常               */
/* 1 : connect 超时       */
/* 2 : 异常               */
/**************************/
int sendSocket(char *buffer, int bufferLen, char *type)
{
    signal( SIGPIPE, SIG_IGN );

    int result = -100;
    errno = 0;



    debug_msg ("type = %s\n", type);

    /* pthread_mutex_lock(&g_pthTCPSocket); */

    if (pthread_mutex_trylock(&g_pthTCPSocket) != 0){
      debug_msg("result = -2");
        result = -2;             /* if trylock failed, then return, and not to send/write buffer.  */
    }else{
        if (g_isCreated) {      /* if is created, then send/write this buffer */
            if (write(clientModelTCPSocket, buffer, bufferLen) != -1){
                result = 0;
                debug_msg("0");
                pthread_mutex_unlock(&g_pthTCPSocket); 
            } else {
                result = -1;
                debug_msg("-1");
                pthread_mutex_unlock(&g_pthTCPSocket); 
            }
        } else {                /* not create socket, but it is not this function's business. so not to send/write. just return */
            result = -2;
            pthread_mutex_unlock(&g_pthTCPSocket);
            debug_msg("-2");
        }
    }

    
  ret:
    if (result == -100)
    {
        result = 2;
    }



    if (result == -1)
    {
#ifdef RUN
        debug_msg("--");
#endif
        /* start thread */

        /* startCheckConnection(); */
        if (g_isChecking)
        {
            debug_msg ("g_bIsChecking is Running, don't start again.\n");
        }
        else
        {
            debug_msg ("g_bIsChecking is not Running, should start it.\n");
            sem_post(&g_semConnectionCheck);
        }
    }

    debug_msg ("sendSocket Result = %d\n", result);

//    free(buffer);               /* release buffer */

    return result;
}
