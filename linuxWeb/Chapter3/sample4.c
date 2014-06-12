/*
 * =====================================================================================
 *
 *    Filename:  sample4.c
 *
 * Description:  Linux网络编程 第三章
 *
 *    
 *
 *     Version:  0.1
 *     Created:  03 May 2014  14:28:35
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   03 May 2014  14:28:35 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 

static union {
    char c[4];
    unsigned long l;
}endian_test = { { 'l', '?', '?', 'b' } };
 
#define ENDIANNESS ((char)endian_test.l)

/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


int main(int argc, char *argv[])
{
    if (ENDIANNESS == 'b')
    {
        printf("big\n");
    }
    else if(ENDIANNESS == 'l')
    {
        printf("little\n");
    }
    else
    {
        printf("error\n");
    }

    int pid, status, exit_status;
    printf ("%d\n",sizeof(pid));
    if ((pid = fork()) < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (! pid)
    {
//        sleep(4);
        int child = 0xaabbccdd;
        int i = 0;
        for (i = 0; i < 32; i++)
        {
            printf ("%d ", (child >> i) & 0x1);
        }
        printf (" \n");
        exit(0xaabbccdd);
        /* 如果返回aabbccdd。则父进程得到0x00dd。 */
        exit(5);
    }
      /* wait用来等待子进程的结束，并把子进程的返回值保存到 &status指针所指向的地址中。wait参数是一个int指针（估计当时编写此函数时，int占用2个字节，而不是现在32位机的4字节），子进程返回值的低8位，并保存到双字节地址的高8位。 需要自己手动移位操作，来得到真正的返回值。也可以用类似WEXITSTATUS之类的系统宏来得到。*/
    if (wait(&status) < 0)
    {
        perror("wait failed");
        exit(1);
    }

    printf ("status = %d\n", status);
    printf ("WEXITSTATUS = %d\n", WEXITSTATUS(status));
    int i = 0;
    for (i = 0; i < sizeof(int) * 8; i++)
    {
        printf ("%d ", (status >> i) & 0x1);
    }
    printf (" \n");

    long long a = status;
    printf ("a = %lld\n", a);
    for (i = 0; i < sizeof(long long) * 8; i++)
    {
        printf ("%lld ", (a >> i) & 0x1);
    }
    printf (" \n");

    int b = status >> 8;
    printf ("b = %d\n", b);
    for (i = 0; i < sizeof(int) * 8; i++)
    {
        printf ("%d ", (b >> i) & 0x1);
    }
    printf (" \n");
    

    char c = 1;
    printf ("c = %d\n", c);
    for (i = 0; i < 8; i ++)
    {
        printf ("%d ",(c >> i) &0x1);
    }
    printf (" \n");

    char d = c >> 1;
    printf ("d = %d\n", d);
    char e = c << 1;
    printf ("e = %d\n", e);


    if (status & 0xFF)
    {
        printf ("Somne low-roderbits not zero\n");
    }
    else
    {
        exit_status = status >> 8;
        exit_status &= 0xFF;
        printf ("Exit status from %d was %d \n",pid, exit_status);
    }

    return 0;
}