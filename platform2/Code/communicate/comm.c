/* 
 *     Created:  15 May 2013 14:45:57
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
/* #include "getOtherID.h" */
/* #include "msgqueue.h" */

#include "comm.h"


#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


#include "tool.h"
#include "global.h"
#include "ReadSocketBuffer.h"
#include "ConnectionCheck.h"
#include "../../common/cJSON.h"
#include "sys/reboot.h"
#include "SendHeart.h"
#include "TcpClient.h"
#include "../../common/msgQ.h"





void initial()
{

  /* signal( SIGABRT, SIG_IGN ); */
  /* signal( SIGBUS, SIG_IGN ); */
  /* signal( SIGFPE, SIG_IGN ); */
  /* signal( SIGILL, SIG_IGN ); */
  /* signal( SIGIOT, SIG_IGN ); */
  /* signal( SIGQUIT, SIG_IGN ); */
  /* signal( SIGSEGV, SIG_IGN ); */
  /* signal( SIGTRAP, SIG_IGN ); */
  /* signal( SIGXCPU, SIG_IGN ); */
  /* signal( SIGXFSZ, SIG_IGN ); */

  /* signal( SIGPIPE, SIG_IGN ); */
  /* signal( SIGALRM, SIG_IGN ); */
  /* signal( SIGHUP, SIG_IGN ); */
  /* signal( SIGINT, SIG_IGN ); */
  /* signal( SIGKILL, SIG_IGN ); */
  /* signal( SIGPOLL, SIG_IGN ); */
  /* signal( SIGPROF, SIG_IGN ); */
  /* signal( SIGSYS, SIG_IGN ); */
  /* signal( SIGTERM, SIG_IGN ); */
  /* signal( SIGVTALRM, SIG_IGN ); */
  /* signal(SIGCHLD, SIG_IGN); */


  strcpy(g_serverIP, "42.96.168.137");
  g_serverPort = 1883;

  pthread_mutex_init(&g_pthTCPSocket, NULL);

  sem_init(&g_semConnectionCheck, 0, 0);
  //    sem_init(&g_semSendHeart, 0, 0);
  sem_init(&g_semNetcom, 0, 0);


  int msgID = create_dataQueue();
}


int main(int argc,char ** argv)
{
  initial();



  GetMAC("eth0", g_mac);
  debug_msg("g_mac = %s\n", g_mac);
  int temp;
  pthread_t tcpClient;

  if((temp = pthread_create(&tcpClient, NULL, TcpClientRun, NULL)) != 0)
    {
      //        printf("create thread for TcpClientRun failed !\n");
    }
  else
    {
      //        printf("create thread for TcpClientRun successed !\n");
    }

  pthread_t sendHeart;

  if((temp = pthread_create(&sendHeart, NULL, SendHeartRun, NULL)) != 0)
    {
      printf("create thread for SendHeartRun failed !\n");
    }
  else
    {
      printf("create thread for SendHeartRun successed !\n");
    }


  pthread_t readSocketBuffer;

  if((temp = pthread_create(&readSocketBuffer, NULL, ReadSocketBufferRun, NULL)) != 0)
    {
      printf("create thread for ReadSocketBufferRun failed !\n");
    }
  else
    {
      printf("create thread for ReadSocketBufferRun successed !\n");
    }

  pthread_t pthConnectionRun;

  if((temp = pthread_create(&pthConnectionRun, NULL, ConnectionCheckRun, NULL)) != 0)
    {
      printf("create thread for ConnectionCheckRun failed !\n");
    }
  else
    {
      printf("create thread for ConnectionCheckRun successed !\n");
    }


  while (1) {
    sleep(1);
  }
}

