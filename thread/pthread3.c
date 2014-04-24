/*
 * =====================================================================================
 *
 *    Filename:  pthread3.c
 *
 * Description:  测试线程的同步信号量
 *
 *    
 *
 *     Version:  0.1
 *     Created:  22 Apr 2014  14:37:47
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   22 Apr 2014  14:37:47 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>  
#include <semaphore.h>  
#include <errno.h>  

/*------------Local Variables----------- */ 
//定义一些本地变量 
sem_t g_sem;


/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


void *thread1(void *arg)
{
    int value = 0;
    sem_getvalue(&g_sem, &value);
    printf ("thread1 sem value = %d\n", value);
    sem_wait(&g_sem);

    sem_getvalue(&g_sem, &value);
    printf ("thread1 sem value (after sem_wait) = %d\n", value);
    

    int i = 0;
    for (i = 0; i < 10; i ++)
    {
        printf ("start thread1 (%d)\n", i);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    sem_init(&g_sem, 0, 0);
    
    pthread_t pthread1;
    pthread_create(&pthread1, NULL, thread1, NULL);

    int i = 0;
    for (i = 0; i < 10; i ++)
    {
        printf ("sleep (%d)\n", i);
        sleep(1);
    }
    
    int value = 0;
    sem_getvalue(&g_sem, &value);
    printf ("value = %d\n", value);

    sem_post(&g_sem);

    value = 0;
    sem_getvalue(&g_sem, &value);
    printf ("value = %d\n", value);
    


    return 0;
}