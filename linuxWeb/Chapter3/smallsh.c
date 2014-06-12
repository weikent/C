/*
 * =====================================================================================
 *
 *    Filename:  smallsh.c
 *
 * Description:  一个简单的命令行程序，来自linux网络编程
 *
 *    
 *
 *     Version:  0.1
 *     Created:  03 May 2014  16:29:01
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   03 May 2014  16:29:01 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 
#include "smallsh.h"

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
#define debug_msg printf("\n%-30s[%4d]_%-20s : ", __FILE__, __LINE__, __FUNCTION__) /* debug message */

static char inpbuf[MAXBUF], tokbuf[2 * MAXBUF];
static char *ptr = inpbuf, *tok = tokbuf;

int userin(char *p)
{
    int c, count;
    ptr = inpbuf;
    tok = tokbuf;

    /* 显示提示 */
    printf ("%s", p);
    
    for (count = 0;  ;  )
    {
        if ((c = getchar()) == EOF)
        {
            return EOF;
        }
        
        if (count < MAXBUF)
        {
            inpbuf[count ++] = c;
        }

        if (c == '\n' && count < MAXBUF)
        {
            inpbuf[count] = '\0';
            return(count);
        }

        if (c == '\n')
        {
            printf ("smallsh: input line too long.\n");
            count = 0;
            printf ("%s\n", p);
        }
    }
}


int gettok(char *output)
{
    int type;
    
    output = tok;
    
    /* 首先去除空白字符 */
    for (; *ptr == ' ' || *ptr == '\t'; ptr++)
    {
        *tok++ = *ptr;
    }

    switch(*ptr++)
    {
    case '\n':
        type = EOL;
        break;
    case '&':
        type = AMPERSAND;
        break;
    case ';':
        type = SEMICOLON;
        break;
    default:
        type = ARG;
        while(inarg(*ptr))
            *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return type;
}

static char special[] = {' ', '\t', '*', ';', '\n', '\0'};

int inarg(char c)
{
    char *wrk;
    for (wrk = special; *wrk != '\0'; wrk ++)
    {
        if (c == *wrk)
        {
            return 0;
        }
    }
    return 1;
}

int procline()
{
    debug_msg;
    char *arg[MAXARG + 1];
    debug_msg;
    int toktype;
    debug_msg;
    int narg;
    debug_msg;
    printf ("aaaaaaaaaaaaaa\n");
//    int aaa;

    for (narg = 0;  ;  )
    {
    debug_msg;
        switch(toktype = gettok(*arg[narg]))
        {
        case ARG:
            debug_msg;
            if (narg < MAXARG)
            {
                narg ++;
            }
            break;
        case EOL:
        case SEMICOLON:
        case AMPERSAND:
            debug_msg;
//            aaa = (toktype == AMPERSAND) ? BACKGROUND : FOREGROUND;
            if (narg != 0)
            {
                arg[narg] = NULL;
                //              runcommand(arg, aaa);
            }
            if (toktype == EOL)
            {
                return;
            }
            narg = 0;
            break;
        }
    }
}


int runcommand(char **cline, int where)
{
    int pid, exitstat, ret;
    if ((pid = fork()) < 0)
    {
        perror("fork failed");
        return -1;
    }
    
    if (!pid)
    {
        execvp(*cline, cline);
        perror(*cline);
        exit(127);
    }

    if (where == BACKGROUND)
    {
        printf ("[process id %d]\n", pid);
        return 0;
    }

    while((ret = wait(&exitstat)) != pid && ret != -1)
    {
        return (ret == -1 ? -1 : exitstat);
    }
}


char *prompt = "command>";

int main(int argc, char *argv[])
{
    debug_msg;
    while(userin(prompt) != EOF)
    {
               procline();
    }
    return 0;
}