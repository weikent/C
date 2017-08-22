/*
 * =====================================================================================
 *
 *    Filename:  ConnectionCheck.c
 *
 * Description:  网络检查。
 *               1.搜索附近的wifi并连接
 *               2.执行udhcpc来获取ip
 *               3.ping isocket 网站，验证是否可以连通internet。
 *               4.与服务器建立socket连接，验证服务器是否正常工作。
 *
 *    
 *
 *     Version:  0.1
 *     Created:  Fri Apr 11 15:45:39 2014
 *
 *     Authors:  世建 魏 ,  
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   世建 魏  Fri Apr 11 15:45:39 2014 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

//#include <config.h>
#include "ConnectionCheck.h"

// 嵌入式编程需要用到的头文件

#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netdb.h>
#include <math.h>

#include "tool.h"
#include "global.h"
#include "SendSocket.h"


#include <stdio.h>
#include <glob.h>


#define RT_RDM_CMD_READ			0x6B03
#define RT_RDM_CMD_SET_BASE		0x6B0D


int checkConnectAPI();
int checkConnectMQTT();


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


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: ConnectionCheckRun
 *  Description: 线程函数，用于检查网络连接。
 *   Parameters: 
 /// @param arg 
 * 
 * Return Value: 
 *      Created: Fri Apr 11 12:57:29 2014 by世建 魏
 *     Revision: 当多台设备在同一个网络环境中时，如果某种原因导致网络不通，
 *               这多台设备会同时进行网络检查，会对路由器造成很大的影响。
 *               所以需要生成一个随机数（iRandomBasicNum），根据这个随机数
 *               确定一个时间间隔，来保证多个设备在不同的时间点进行网络检查。
 *               可以缓解对路由器的影响。

 *               当每次检查网络失败的时候，增加这个时间间隔，使用公式
 *               随机数 * 2 的N（1,2,3....)次方 N < CHECK_CONNECTION_TIMES
 *               当检查次数 > CHECK_CONNECTION_TIMES时，重置检查次数为0.重新开始检查网络。

 *               当检查网络成功的时候，退出网络检查。

 *               2014/04/29 08:54:04 修改此线程为始终存在的线程，使用线程的信号机制来启动
 *               或停止此线程。避免在设备运行过程中，频繁创建和退出线程。
                
 * =======================================================================================
 * @0.1   世建 魏  Fri Apr 11 12:57:29 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

void *ConnectionCheckRun(void *arg)
{
  pthread_detach(pthread_self());
#ifdef RUN
  debug_msg("connection Check running\n");
#endif

  for ( ;  ;  )
    {
    checkReStart:
      debug_msg("before sem_wait----------------------------------------\n");

      g_isChecking = false;
      sem_wait(&g_semConnectionCheck);
      g_isChecking = true;


      debug_msg("after sem_wait----------------------------------------\n");

      CloseTCPConnection();

      if (deviceModel == AP) {
        debug_msg("current deviceModel is AP");
        continue;
      }else{ // current deviceModel is STATION

        int iCurrentNumOfCheck = 0;

        /* 生成一个随即数。在此随机数的基础上计算每次网络检查的时间间隔 */
        struct timeval tv;
        gettimeofday(&tv,NULL);
        long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        int randSeed = time % 3000;

        srand((unsigned) randSeed);
        long long lli = rand();
        long int iRandomBasicNum = lli % 3000;
        debug_msg("iRandombasicnum = %ld\n", iRandomBasicNum);
        while(1)
          {
            /* 根据检查次数，确定等待的时间间隔 */
            usleep(iRandomBasicNum * 1000 * pow(2.0, iCurrentNumOfCheck));

            iCurrentNumOfCheck ++;

            int ret = 0;
            if (g_dhcpStatus == 0) { //1:静态   0:动态
            }else{
              ret = setDhcpInfo();
              if (ret == 0) {
              }else{
                continue;
              }
            }

            char temIP[LEN_OF_IPADDRESS] = {0};
            GetIP(NAME_OF_NETCARD_OF_CLIENT, temIP);
            if(temIP[0] != 0 || temIP[0] != '\0')
              {
                debug_msg("Client Model GetIP() = %s\n", temIP);
              }
            else
              {
                debug_msg("can not get ip\n");
                deviceStatus = JOIN_ROUTE_FAILED;
                continue;
              }

            if(checkConnectMQTT() == 0){
              break;
            }
          } /* end while */
      }
    } /* end for */
  pthread_exit((void*)0);
}


int checkConnectMQTT(){

  if (isRightIP(g_serverWebsite) > 0)
    {
      debug_msg("isRightIP\n");
      strcpy(g_serverIP, g_serverWebsite);
    }
  else
    {
      struct hostent *host;

      if((host = gethostbyname(g_serverWebsite)) == NULL)
        {
          debug_msg("can not get serverIP\n");
          deviceStatus = CONNECT_SERVER_FILED;
          return -1;
        }

      strcpy(g_serverIP, inet_ntoa(*((struct in_addr *)host->h_addr)));
    }

  errno = 0;

  /* 保证同一时间只有一个线程在操作g_iClientModelTCPSocket */
  pthread_mutex_lock(&g_pthTCPSocket); 
  debug_msg("start lock g_iClientModelTCPSocket------------------------------\n");

  CloseTCPConnection();

  clientModelTCPSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientModelTCPSocket < 0)
    {
      return -1;
    }

  struct sockaddr_in s_add;

  bzero(&s_add,sizeof(struct sockaddr_in));
  s_add.sin_family=AF_INET;
  debug_msg("g_serverIP = %s\n", g_serverIP);
  debug_msg("g_serverPort = %d\n", g_serverPort);
  inet_pton(AF_INET, g_serverIP, &s_add.sin_addr);
  s_add.sin_port=htons(g_serverPort); 

  struct timeval timeout = {10,0};
  socklen_t len = sizeof(timeout);
  setsockopt(clientModelTCPSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);

  errno = 0;
  if(0 != connect(clientModelTCPSocket,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {

      debug_msg ("clientModelTCPSocket = %d\n", clientModelTCPSocket);
      CloseTCPConnection();
      debug_msg("clientModelTCPSocket = %d\n", clientModelTCPSocket);

      pthread_mutex_unlock(&g_pthTCPSocket);
      debug_msg("start unlock g_pthTCPSocket------------------------------\n");

      return -1;
    }
  else
    {
      printf ("clientModelTCPSocket = %d\n", clientModelTCPSocket);

      g_isCreated = true;

      //sem_post(&g_semSendHeart);

      pthread_mutex_unlock(&g_pthTCPSocket);
      debug_msg("start unlock g_pthTCPSocket------------------------------\n");
      return 0;
    }

  return 0;
}
