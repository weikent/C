/*
 * =====================================================================================
 *
 *    Filename:  timer.c
 *
 * Description:  这个文件用于计时， 对于不同的命令有不同的延时。
 *
 *    
 *
 *     Version:  0.1
 *     Created:  13 Mar 2017  15:02:17
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   13 Mar 2017  15:02:17 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 
#include "timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "global.h"

#include <unistd.h>             /* for usleep */


/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


void *timer(void *args)
{
  pthread_detach(pthread_self());

  while (1) {
    sem_wait(&g_semHasCommand);

    usleep(5000);

    sem_post(&g_semNeedWait);
  }

  pthread_exit((void*)0);

}
