/*
 * =====================================================================================
 *
 *    Filename:  main.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  27 Jun 2017  16:40:38
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   27 Jun 2017  16:40:38 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件
#include "mosquitto.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */
//定义一些本地变量



/*------Local Structures and Typedefs ---*/
//要使用的一些数据结构



/*-----------Extern Variables -----------*/
//使用到的一些外部变量



/*-------------Definitions---------------*/
//一些#defines及具体的函数实现


int main(int argc, char *argv[])
{
  int major, minor, revision;
  int a = mosquitto_lib_version(&major, &minor, &revision);
  printf("major = %d, minor= %d, revision = %d\n", major, minor, revision);
  return 0;
}
