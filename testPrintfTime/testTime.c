/*
 * =====================================================================================
 *
 *    Filename:  testTime.c
 *
 * Description:  test printf' time.
 *
 *    
 *
 *     Version:  0.1
 *     Created:  09 May 2014  14:04:40
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   09 May 2014  14:04:40 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 

#include <time.h>
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
    char test[1000] = {0};
    int i = 0;
    for (i = 0; i < 1000; i ++)
    {
        test[i] = 'a';
    }

    struct timeval tv;  
    gettimeofday(&tv,NULL);
    long long time_sec = tv.tv_sec;
    int time_usec = (int)(tv.tv_usec / 1000);
    printf ("%lld%03d\n", time_sec, time_usec);
    
    printf ("%s\n", test);

    struct timeval tv1;  
    gettimeofday(&tv1,NULL);
    time_sec = tv1.tv_sec;
    time_usec = (int)(tv1.tv_usec / 1000);
    printf ("%lld%03d\n", time_sec, time_usec);


    struct timeval tv2;  
    gettimeofday(&tv2,NULL);
    time_sec = tv2.tv_sec;
    time_usec = (int)(tv2.tv_usec / 1000);
    printf ("%lld%03d\n", time_sec, time_usec);


    for (i = 0; i < 1000; i ++)
    {
        printf ("%02x", test[i]);
    }

    printf (" \n");
    struct timeval tv3;  
    gettimeofday(&tv3,NULL);
    time_sec = tv3.tv_sec;
    time_usec = (int)(tv3.tv_usec / 1000);
    printf ("%lld%03d\n", time_sec, time_usec);


    return 0;
}