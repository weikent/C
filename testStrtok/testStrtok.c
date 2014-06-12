/*
 * =====================================================================================
 *
 *    Filename:  testStrtok.c
 *
 * Description:  测试strtok
 *
 *    
 *
 *     Version:  0.1
 *     Created:  07 May 2014  09:23:38
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   07 May 2014  09:23:38 , create orignal file
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
    char testStr[] = "aaaaa=";


    char *key[2] = {0};
    printf ("testStr = %s\n", testStr);
    key[0] = strtok(testStr, "=");
    printf ("testStr = %s\n", testStr);

    key[1] = strtok(NULL, "=");

    printf ("testStr = %s\n", testStr);
    return 0;
}