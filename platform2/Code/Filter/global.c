/**
 *   \file global.c
 *   \brief A Documented file.
 *
 *  Detailed description
 *
 *
 * Fri Mar 10 14:43:58 2017
 *
 * author:魏世建
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 


#include "global.h"

int msgID = 0;

sem_t g_semNeedWait;
sem_t g_semHasCommand;

