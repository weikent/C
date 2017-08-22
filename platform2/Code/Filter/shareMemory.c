/*
 * =====================================================================================
 *
 *    Filename:  shareMemory.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  18 Aug 2017  11:10:01
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   18 Aug 2017  11:10:01 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件
#include "shareMemory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>




/*------------Local Variables----------- */
//定义一些本地变量



/*------Local Structures and Typedefs ---*/
//要使用的一些数据结构



/*-----------Extern Variables -----------*/
//使用到的一些外部变量



/*-------------Definitions---------------*/
//一些#defines及具体的函数实现



int initial_shareMemory(int key, struct shared_use_st **shared)
{


  int shmid = shmget((key_t)key, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
  if (shmid == -1) {
    debug_msg("initialize share memory failed!");
    return -1;
  }

  void *memoryAdd = shmat(shmid, 0, 0);

  if (memoryAdd == (void*)-1) {
    debug_msg("set address failed!");
    return -1;
  }

  *shared = (struct shared_use_st*)memoryAdd;
  (*shared)->written = 0;

  return 0;
}



void *shareMemoryRead(void *args)
{
  pthread_detach(pthread_self());

  while (1) {
    if (g_sharedRead->written == 1) {
      //TODO: Read
      //TODO: Deal Data
    }else{
      // do nothing
      usleep(5000);
    }
  }

  pthread_exit((void*)0);

}


void *shareMemoryWrite(void *args){
  pthread_detach(pthread_self());

  while(1){
    if (g_sharedWrite->written == 0 && g_sharedWriteBuf[0] == 0xFF) {
      pthread_mutex_lock(&g_pthSharedWrite);
      // TODO: Write data, which got from uart0 and server
      memcpy(g_sharedWrite->text, g_sharedWriteBuf, 5);

      memset(g_sharedWriteBuf, 0, SHARED_TEXT_SIZE);
      /* printf("%02X \n", g_sharedWriteBuf[0]); */
      /* printf("%02X \n", g_sharedWriteBuf[1]); */
      /* printf("%02X \n", g_sharedWriteBuf[2]); */
      /* printf("%02X \n", g_sharedWriteBuf[3]); */
      /* printf("%02X \n", g_sharedWriteBuf[4]); */

      /* printf("%02X \n", g_sharedWrite->text[0]); */
      /* printf("%02X \n", g_sharedWrite->text[1]); */
      /* printf("%02X \n", g_sharedWrite->text[2]); */
      /* printf("%02X \n", g_sharedWrite->text[3]); */
      /* printf("%02X \n", g_sharedWrite->text[4]); */

      g_sharedWrite->written = 1;
      pthread_mutex_unlock(&g_pthSharedWrite);
      sleep(1);
    }else{
      usleep(5000);
    }
  }
  pthread_exit((void*)0);
}
