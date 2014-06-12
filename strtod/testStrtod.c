/*
 * =====================================================================================
 *
 *    Filename:  testStrtod.c
 *
 * Description:  
 *
 *    
 *
 *     Version:  0.1
 *     Created:  10 May 2014  11:18:23
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   10 May 2014  11:18:23 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 

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
//    char c[] = "4e1fc9fbee77f7bc8a5770cc8f93bbda";
    char c[] = "ffff";
    char cc[32] = {0};
    printf ("%ld\n",strtol(c,NULL, 16));
    printf ("%s\n", cc);

    return 0;
}