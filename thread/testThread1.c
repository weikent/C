/*
 * =====================================================================================
 *
 *    Filename:  testThread1.c
 *
 * Description:  此文件用来测试线程，
 *               使用thread_join来实现多个线程按创建的顺序来执行
 *
 *    
 *
 *     Version:  0.1
 *     Created:  15 Apr 2014  16:37:29
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   15 Apr 2014  16:37:29 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现

int iTempThreadID = 0;
void *Thread(void *arg)
{
    printf ("%ld\n", pthread_self());
/*     if (iTempThreadID == 0) */
/*     {} */
/*     else */
/*     { */
/* //        pthread_join(iTempThreadID, NULL); */
/*     } */
/*     iTempThreadID = pthread_self(); */

    int i = 0;
    for (i = 0; i < 100; ++i)
    {
        printf ("%ld : i = %d\n", pthread_self(), i);
    }
    
}

int main(int argc, char *argv[])
{
    pthread_t liThreadID;
    int iCreateResult = 0;

    iCreateResult = pthread_create(&liThreadID, NULL, Thread, NULL);
    usleep(20);
    printf ("iCreateResult = %d\n", iCreateResult);
    printf ("liThreadID = %ld\n", liThreadID);
    
    iCreateResult = pthread_create(&liThreadID, NULL, Thread, NULL);
    usleep(20);
    printf ("iCreateResult = %d\n", iCreateResult);
    printf ("liThreadID = %ld\n", liThreadID);

    iCreateResult = pthread_create(&liThreadID, NULL, Thread, NULL);
    usleep(20);
    printf ("iCreateResult = %d\n", iCreateResult);
    printf ("liThreadID = %ld\n", liThreadID);


    sleep(3);
    return 0;
}
