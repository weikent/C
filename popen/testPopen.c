/*
 * =====================================================================================
 *
 *    Filename:  testPopen.c
 *
 * Description:  
 *
 *    
 *
 *     Version:  0.1
 *     Created:  10 May 2014  11:07:32
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   10 May 2014  11:07:32 , create orignal file
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
    FILE *pf;
    char command[40];
    char data[40];
 
    // Execute a process listing
    sprintf(command, "md5sum testPopen.c"); 
 
    // Setup our pipe for reading and execute our command.
    pf = popen(command,"r"); 
 
    if(!pf){
      fprintf(stderr, "Could not open pipe for output.\n");
      return -1;
    }
 
    // Grab data from process execution
    fgets(data, 40 , pf);
 
    // Print grabbed data to the screen.
    fprintf(stdout, "-%s-\n",data); 
 
    if (pclose(pf) != 0)
        fprintf(stderr," Error: Failed to close command stream \n");
 
    return 0;
}