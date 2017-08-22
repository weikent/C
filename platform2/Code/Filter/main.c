/**
 * @file   main.c
 * @brief  this is the main file of platform2(7688 chip)
 *
 * <long description> 测试版本，用来测试7688平台的可行性。
 *
 * @author weikent
 * @date   2017-03-09
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/stat.h>

#include "global.h"
#include "../../common/msgQ.h"
#include "socketSerialPort.h"
#include "timer.h"
#include "protocol.h"

#include "../../common/message.h"
#include "shareMemory.h"


/* struct command{ */
/*   char command; */
/*   unsigned char buf[SHARED_TEXT_SIZE]; */
/*   int len; */
/* }; */

unsigned char commands[][5]={
  {0xFF, CMD_GET_TEMP_HUMID,       0x02, 0x54, 0x50},
  {0xFF, CMD_GET_TEMP_HUMID,       0x02, 0x54, 0x50},
  {0xFF, CMD_OPEN_COOL,       0x00, 0x54, 0x57},
  {0xFF, CMD_OPEN_HEAT,       0x00, 0x57, 0x56},
  {0xFF, CMD_OPEN_FAN_HIGH,   0x00, 0x56, 0x51},
  {0xFF, CMD_OPEN_FAN_MIDDLE, 0x00, 0x51, 0x50},
  {0xFF, CMD_OPEN_FAN_LOW,    0x00, 0x50, 0x53},
  {0xFF, CMD_CLOSE_COOL,      0x00, 0x53, 0x52},
  {0xFF, CMD_CLOSE_HEAT,      0x00, 0x52, 0x5D},
  {0xFF, CMD_CLOSE_FAN,       0x00, 0x5D, 0x5C},
  {0xFF, CMD_CLOSE_ALL,       0x00, 0x5C, 0x5F},
  {0xFF, CMD_OPEN_FAN,        0x00, 0x5F, 0x5E}
};
/* struct command commands[] = { */
/*   {CMD_OPEN_COOL,          {0xFF, CMD_OPEN_COOL,       0x00, 0x54, 0x57},5}, */
/*   {CMD_OPEN_HEAT,          {0xFF, CMD_OPEN_HEAT,       0x00, 0x57, 0x56},5}, */
/*   {CMD_OPEN_FAN_HIGH,      {0xFF, CMD_OPEN_FAN_HIGH,   0x00, 0x56, 0x51},5}, */
/*   {CMD_OPEN_FAN_MIDDLE,    {0xFF, CMD_OPEN_FAN_MIDDLE, 0x00, 0x51, 0x50},5}, */
/*   {CMD_OPEN_FAN_LOW,       {0xFF, CMD_OPEN_FAN_LOW,    0x00, 0x50, 0x53},5}, */
/*   {CMD_CLOSE_COOL,         {0xFF, CMD_CLOSE_COOL,      0x00, 0x53, 0x52},5}, */
/*   {CMD_CLOSE_HEAT,         {0xFF, CMD_CLOSE_HEAT,      0x00, 0x52, 0x5D},5}, */
/*   {CMD_CLOSE_FAN,          {0xFF, CMD_CLOSE_FAN,       0x00, 0x5D, 0x5C},5}, */
/*   {CMD_CLOSE_ALL,          {0xFF, CMD_CLOSE_ALL,       0x00, 0x5C, 0x5F},5}, */
/*   {CMD_OPEN_FAN,           {0xFF, CMD_OPEN_FAN,        0x00, 0x5F, 0x5E},5} */
/* }; */

void init(){
  msgID = create_dataQueue();
  debug_msg("msgID = %d", msgID);

  sem_init(&g_semHasCommand, 0, 0);
  sem_init(&g_semNeedWait, 0, 0);


  //  init_serial();

  int ret = initial_shareMemory(100, &g_sharedRead);
  ret = initial_shareMemory(101, &g_sharedWrite);

  pthread_mutex_init(&g_pthSharedWrite, NULL);
  pthread_mutex_init(&g_pthMsgWrite, NULL);
}


/**
 * this function is used to get data from other app, for example, comm, web.
 * and send transfer it , and send to uart0 and sharememory.
 *
 * @param args
 */
  void *getDataFromQueue(void *args)
  {
    pthread_detach(pthread_self());

    while(1){
      bzero(g_msgReadMsgBuf.msg_text, sizeof(g_msgReadMsgBuf.msg_text));
      read_message(msgID, &g_msgReadMsgBuf, MY_MSG_TYPE);

      int cmd = atoi(g_msgReadMsgBuf.msg_text);

      debug_msg("cmd = %d", cmd);

      int i = 0;
      for (i = 0; i < 5; i++) {
        printf("%02X ", commands[cmd][i]);
      }
      printf("\n");
      //TODO: transfer those data for example "1" to FF 01 00 XX XX



      // do not need to send to uart0 anymore.
      /*  Step1 write those transferred data to uart0 */
      /* bzero(g_serialMessageBuf, MAX_SERIAL_MSG_SIZE + 1); */
      /* uart_send(serial_fd, g_msgReadMsgBuf.msg_text, 1); /\* TODO:  需要先解析消息队列中的信息，然后再发送 *\/ */


      //TODO: Step2 write those transferred data to share memory.
      if (pthread_mutex_trylock(&g_pthSharedWrite) != 0) {
        debug_msg("g_pthSharedWrite is locked");
      }else{
        memset(g_sharedWriteBuf, 0, SHARED_TEXT_SIZE);
        memcpy(g_sharedWriteBuf, commands[cmd], 5);
        /* for (i = 0; i < 5; i++) { */
        /*   printf("%02X ", g_sharedWriteBuf[i]); */
        /* } */
        pthread_mutex_unlock(&g_pthSharedWrite);
      }
    }
    pthread_exit((void*)0);

  }

int main(int argc, char *argv[])
{

  init();

  int temp;
  /* pthread_t pt_shareRead; */

  /* if((temp = pthread_create(&pt_shareRead, NULL, shareMemoryRead, NULL)) != 0){ */
  /*     debug_msg("start shareMemoryRead"); */
  /* } */
  /* else{ */
  /* } */

  pthread_t pt_shareWrite;
  if((temp = pthread_create(&pt_shareWrite, NULL, shareMemoryWrite, NULL)) != 0){
    debug_msg("start shareMemoryWrite");
  }else{
  }


  pthread_t pt_getDataFromQueue;
  if ((temp = pthread_create(&pt_getDataFromQueue, NULL, getDataFromQueue, NULL)) !=0) {
    debug_msg("start getDataFromQueue");
  }else{
  }



  while(1){

    /* bzero(g_msgReadMsgBuf.msg_text, sizeof(g_msgReadMsgBuf.msg_text)); */
    /* read_message(msgID, &g_msgReadMsgBuf, MY_MSG_TYPE); */

    /* debug_msg("the message is : %s,  the sender is : %d", g_msgReadMsgBuf.msg_text, g_msgReadMsgBuf.sender_type); */
    /* bzero(g_serialMessageBuf, MAX_SERIAL_MSG_SIZE + 1); */
    /* uart_send(serial_fd, g_msgReadMsgBuf.msg_text, 1); /\* TODO:  需要先解析消息队列中的信息，然后再发送 *\/ */

    /* sem_post(&g_semHasCommand); */
    /* sem_wait(&g_semNeedWait); */

    /****************************************************************************/
    /* bzero(g_serialMessageBuf, MAX_SERIAL_MSG_SIZE + 1);                      */
    /* int len = uart_recv(serial_fd, g_serialMessageBuf, MAX_SERIAL_MSG_SIZE); */
    /* if (len <= 0) {                                                          */
    /*   debug_msg("uart_recv failed, maybe timeout");                          */
    /*   continue;                                                              */
    /* }                                                                        */
    /*                                                                          */
    /*                                                                          */
    /* if (pthread_mutex_trylock(&g_pthSharedWrite) != 0) {                     */
    /*     debug_msg("g_pthSharedWrite is locked");                             */
    /*                                                                          */
    /*   }else{                                                                 */
    /*                                                                          */
    /*   memset(g_sharedWriteBuf, 0, SHARED_TEXT_SIZE);                         */
    /*   memcpy(g_sharedWriteBuf, g_serialMessageBuf, len);                     */
    /*   pthread_mutex_unlock(&g_pthSharedWrite);                               */
    /*                                                                          */
    /* }                                                                        */
    /****************************************************************************/


    /* int i = 0; */
    /* for (i = 0; i < len; i++) { */
    /*   printf("%02X ", g_serialMessageBuf[i]); */
    /* } */

    /* printf("\n" ); */

    /* int isVaildBuf = CheckData(g_serialMessageBuf, len -2); */
    /* debug_msg("isVaildBuf = %d", isVaildBuf); */

    /* char temperature = g_serialMessageBuf[3]; */
    /* char humidity = g_serialMessageBuf[4]; */
    /* debug_msg("temperature = %d, humidity = %d", temperature, humidity); */



    if (g_sharedRead->written == 1) {
      //TODO: Read
      //TODO: Deal Data
      g_sharedRead->written = 0;

      int len = 0;
      char cmd = -1;
      if ((g_sharedRead->text)[0] == 0xFF) {
        cmd = (g_sharedRead->text)[1];
        if (cmd == 0x01) {
          len = 7;
        }else{
          len = 5;
        }
      }else{
        continue;
      }


      int isVaildBuf = CheckData(g_sharedRead->text, len - 2);
      if (isVaildBuf == 1) {
        unsigned char message[100] = {0}; // TODO: need to change 100
        if (cmd == 0x01) {
          char temperature = g_sharedRead->text[3];
          char humidity = g_sharedRead->text[4];
          sprintf(message, "temperature = %d, humidity = %d", temperature, humidity);
        }else{
          sprintf(message, "command = %d", cmd);
        }

        pthread_mutex_lock(&g_pthMsgWrite);
        g_msgWriteMsgBuf.sender_type = MY_MSG_TYPE;
        strncpy(g_msgWriteMsgBuf.msg_text, message, strlen(message));
        g_msgWriteMsgBuf.msg_type = COMM_MSG_TYPE;
        send_message(msgID, &g_msgWriteMsgBuf, COMM_MSG_TYPE, message);

        pthread_mutex_unlock(&g_pthMsgWrite);
      }else{
        debug_msg("data is unvailable");
        continue;
      }


    }else{
      // do nothing
      usleep(5000);
    }


    // TODO: need to send a right receiver. according to the buf contains.

  }

  return 0;
}

