/*
 * =====================================================================================
 *
 *    Filename:  sample3.c
 *
 * Description:  linux网络编程 第三章
 *
 *    
 *
 *     Version:  0.1
 *     Created:  03 May 2014  09:34:01
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   03 May 2014  09:34:01 , create orignal file
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
    int pid;
    pid = fork();
    switch(pid)
    {
    case -1:
        perror("fork failed");
        exit(1);
    case 0:
        printf ("I'm child\n");
        execl("/bin/ls", "ls", "-l", "--color", NULL);
        perror("execl failed");
        exit(1);
    default:
        printf ("I'm parent,\n");
        wait(NULL);
        printf ("ls completed \n");
        exit(0);
    }
    return 0;
}