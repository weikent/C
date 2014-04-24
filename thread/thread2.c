/*
 * =====================================================================================
 *
 *    Filename:  thread2.c
 *
 * Description:  
 *
 *    
 *
 *     Version:  0.1
 *     Created:  22 Apr 2014  14:24:44
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   22 Apr 2014  14:24:44 , create orignal file
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


#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <errno.h>  
 
#define return_if_fail(p)  / if((p) == 0){printf ("[%s]: func error!", __func__);return;}  
  
typedef struct _PrivInfo  
{  
    sem_t    sem;  
    int      lock_var;  
    time_t   end_time;  
}PrivInfo;  
  
static void info_init (PrivInfo* thiz);  
static void* pthread_func_1 (PrivInfo* thiz);  
static void* pthread_func_2 (PrivInfo* thiz);  
  
int main (int argc, char** argv)  
{  
    pthread_t pt_1 = 0;  
    pthread_t pt_2 = 0;  
    int ret = 0;  
    PrivInfo* thiz = NULL;  
  
    thiz = (PrivInfo* )malloc (sizeof (PrivInfo));  
    if (thiz == NULL)  
    {  
        printf ("Failed to malloc PrivInfo./n");  
        return -1;  
    }  
  
    printf ("aaaaa\n");
    info_init(thiz);  
    
    printf ("bbbbb\n");
    ret = pthread_create (&pt_1, NULL, (void*)pthread_func_1, thiz);  
    if (ret != 0)  
    {  
        perror ("pthread_1_create:");  
    }  
  
    printf ("ccccc\n");
    

    ret = pthread_create (&pt_2, NULL, (void*)pthread_func_2, thiz);  
    if (ret != 0)  
    {  
        perror ("pthread_2_create:");  
    }  
  
    pthread_join (pt_1, NULL);  
    pthread_join (pt_2, NULL);  
  
    sem_destroy (&thiz->sem);  
    free (thiz);  
    thiz = NULL;  
  
    return 0;  
}  
  
static void info_init (PrivInfo* thiz)  
{  
//    if((thiz != NULL) == 0){printf ("[%s]: func error!", __func__);return;}  
//    return_if_fail(thiz != NULL);  
  
    thiz->lock_var = 0;  
    thiz->end_time = time(NULL) + 10;  
  
    sem_init (&thiz->sem, 0,  1);  
  
    return;  
}  
  
static void* pthread_func_1 (PrivInfo* thiz)  
{  

//    if((thiz != NULL) == 0){printf ("[%s]: func error!", __func__);return;}  
//    return_if_fail(thiz != NULL);  
  
    int i = 0;  
  
    while (1)  
    {  
        printf ("ddddd\n");
        sem_wait (&thiz->sem);  
        printf ("pthread: pthread1 get lock./n");  
  
        for (i = 0; i < 2; i ++)  
        {  
            thiz->lock_var ++;  
            sleep (1);  
        }  
  
        sem_post (&thiz->sem);  
        printf ("pthread1: pthread1 unlock/n");  
  
        sleep (1);  
    }  
  
    return;  
}  
  
static void* pthread_func_2 (PrivInfo* thiz)  
{  
//    if((thiz != NULL) == 0){printf ("[%s]: func error!", __func__);return;}  
//  return_if_fail (thiz != NULL);  
  
    while (1)
    {  
        sem_wait (&thiz->sem);  
        printf ("pthread2: pthread2 get lock!/n");  
        printf ("the lock_var = %d/n", thiz->lock_var);  
  
        sem_post (&thiz->sem);  
        printf ("phtread2: pthread2 unlock./n");  
  
        sleep (3);  
    }  
  
    return;  
}  
