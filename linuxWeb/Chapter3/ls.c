/*
 * =====================================================================================
 *
 *    Filename:  ls.c
 *
 * Description:  linux网络编程 第三章
 *
 *    
 *
 *     Version:  0.1
 *     Created:  03 May 2014  09:05:07
 *
 *     Authors:  weishijian
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   03 May 2014  09:05:07 , create orignal file
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    printf ("Executing ls \n");

    execl("/bin/ls", "ls", "-l", NULL);
    /* 如果execl返回，说明调用失败 */
    perror("execl failed to run ls");
    exit(0);

}