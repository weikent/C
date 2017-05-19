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
#include "DeviceModelChange.h"
#include "../../common/dealJson.h"




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


  init_serial();




  strcpy(g_serverIP, "42.96.168.137");
  g_serverPort = 1883;

  pthread_mutex_init(&g_pthTCPSocket, NULL);

  sem_init(&g_semConnectionCheck, 0, 0);
  //    sem_init(&g_semSendHeart, 0, 0);

  int ret = -1;
  if ((ret = getValueFromJson(CONFIGFILE, "ssid", g_ssid)) == 0) {
    debug_msg("g_ssid = %s", g_ssid); 
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "password", g_password)) == 0) {
    debug_msg("g_password = %s", g_password);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "ip", g_ip_addr)) == 0) {
    debug_msg("g_ip_addr = %s", g_ip_addr);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "ip", g_ip_addr)) == 0) {
    debug_msg("g_ip_addr = %s", g_ip_addr);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "mask", g_mask)) == 0) {
    debug_msg("g_mask= %s", g_mask);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "gateway", g_gateWay)) == 0) {
    debug_msg("g_gateWay= %s", g_gateWay);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "dns1", g_dns1)) == 0) {
    debug_msg("g_dns1 = %s", g_dns1);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "dns1", g_dns2)) == 0) {
    debug_msg("g_dns2 = %s", g_dns2);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  if ((ret = getValueFromJson(CONFIGFILE, "serverIP", g_serverWebsite)) == 0) {
    debug_msg("g_serverIP = %s", g_serverWebsite);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  char cServerPort[6] = {0};
  if ((ret = getValueFromJson(CONFIGFILE, "serverPort", cServerPort)) == 0) {
    g_serverPort = atoi(cServerPort);
    debug_msg("g_serverPort = %d", g_serverPort);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  char cDhcpStatus[2] = {0};
  if ((ret = getValueFromJson(CONFIGFILE, "dhcp", cDhcpStatus)) == 0) {
    g_dhcpStatus = atoi(cDhcpStatus);
    debug_msg("g_dhcpStatus = %d", g_dhcpStatus);
  }else{
    debug_msg("read %s failed, should change to AP model!", CONFIGFILE);
    changeToAP();
  }

  int msgID = create_dataQueue();
}


int createHeart(){
  GetMAC("apcli0", g_mac);
  debug_msg("g_mac = %s\n", g_mac);
  //  strcpy(g_mac, "DDDDDDDDDDDDDDDDDDDDDDeDDDDDDDDD");

  char heart[100] = {0};
  sprintf(heart, "{\"optCode\":\"heart\",\"devID\":\"%s\"}", g_mac);
  debug_msg ("Heart before encry : %s\n", heart);


  if (needEncyrpt == 1) {
    int clientMessageLen = encry(heart, strlen(heart), key1, key2, key3);

    bool bbreakLine=false;
    unsigned long cbB64Len, cbBytes;
    char *szBase64;
    debug_msg("--");
    cbBytes = clientMessageLen;

    //計算所需記憶體空間
    cbB64Len = (cbBytes+2)/3*4+1;
    if (bbreakLine) cbB64Len += cbB64Len/76*2;
    //配置記憶體空間
    debug_msg("--");
    szBase64 = (char*)malloc(cbB64Len);
    //呼叫編碼參數說明:szBase64=編碼結果;pbData=原始資料;
    //cbBytes=資料長度;bbreakLine=自動換行(true/false);
    //若bbreakLine設為true則每76個字即插入換行符號
    //若為false則輸出不換行之連續長字串
    debug_msg("--");
    int base64Len= b64encode(szBase64, heart, cbBytes, bbreakLine);
    // printf("\nbase64 encoded:\n%s\n", szBase64);
    // printf("cbB64len = %d\njj = %d\n", cbB64Len, base64Len);
    // int i = 0;
    // for (i = 0; i < base64Len; i++) {
    //     printf("%02X,", szBase64[i]);
    // };
    // printf("\n" );

    mqttHeart = NULL;
    mqttHeartLen =  mqttPublish(base64Len, szBase64, &mqttHeart);

    debug_msg("--");

    free(szBase64);
  }else{
    mqttHeart = NULL;
    mqttHeartLen = mqttPublish(strlen(heart), heart, &mqttHeart);
  }
  int i = 0;
  for (i = 0; i < mqttHeartLen; i++) {
    printf("%02X,", mqttHeart[i]);
  }
  printf("\n");
 
}


int main(int argc,char ** argv)
{
  initial();
  createHeart();

  if (g_serverWebsite[0] != 0 && g_serverPort != 0 && g_ssid[0] != 0 && strcmp(g_ssid, "empty") != 0)
    {
      changeToSta();
      deviceStatus = CONNECT_SERVER_FILED;
    }
  else
    {
      changeToAP();
    }


  int temp;
  pthread_t deviceModel;
  if ((temp = pthread_create(&deviceModel, NULL, DeviceModelChange, NULL)) != 0) {
    debug_msg("create thread for DeviceModelChange failed !");
  }
  else{
    debug_msg("create thread for DeviceModelChange successed !");
  }

  pthread_t tcpClient;

  if((temp = pthread_create(&tcpClient, NULL, TcpClientRun, NULL)) != 0)
    {
      debug_msg("create thread for TcpClientRun failed !\n");
    }
  else
    {
      debug_msg("create thread for TcpClientRun successed !\n");
    }

  pthread_t sendHeart;

  if((temp = pthread_create(&sendHeart, NULL, SendHeartRun, NULL)) != 0)
    {
      debug_msg("create thread for SendHeartRun failed !\n");
    }
  else
    {
      debug_msg("create thread for SendHeartRun successed !\n");
    }


  pthread_t readSocketBuffer;

  if((temp = pthread_create(&readSocketBuffer, NULL, ReadSocketBufferRun, NULL)) != 0)
    {
      debug_msg("create thread for ReadSocketBufferRun failed !\n");
    }
  else
    {
      debug_msg("create thread for ReadSocketBufferRun successed !\n");
    }

  pthread_t pthConnectionRun;

  if((temp = pthread_create(&pthConnectionRun, NULL, ConnectionCheckRun, NULL)) != 0)
    {
      debug_msg("create thread for ConnectionCheckRun failed !\n");
    }
  else
    {
      debug_msg("create thread for ConnectionCheckRun successed !\n");
    }


  struct myMsgBuf msgReadMsgBuf;
  while (1) {
    /* bzero(msgReadMsgBuf.msg_text, sizeof(msgReadMsgBuf.msg_text)); */
    /* read_message(msgID, &msgReadMsgBuf, MY_MSG_TYPE); */

    /* debug_msg("the message is : %s,  the sender is : %d", msgReadMsgBuf.msg_text, msgReadMsgBuf.sender_type); */
    sleep(2);

    if (g_isSubscribed) {

      msgReadMsgBuf.msg_type = FILTER_MSG_TYPE;
      msgReadMsgBuf.sender_type = MY_MSG_TYPE;

      int i = 0;
      for (i = 0; i < 1; i++) {
        bzero(msgReadMsgBuf.msg_text, sizeof(msgReadMsgBuf.msg_text));
        char command[2] = {0};
        sprintf(command, "%d", i);
        strcpy(msgReadMsgBuf.msg_text, command);
        send_message(msgID, &msgReadMsgBuf, FILTER_MSG_TYPE, command);
      }

    }

  }
}

