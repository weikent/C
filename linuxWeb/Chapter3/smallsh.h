// -*- C++ -*-

/* @(#)smallsh.h
 */

#ifndef _SMALLSH_H
#define _SMALLSH_H 1

#include <stdio.h>
#define EOL 1
#define ARG 2
#define AMPERSAND 3
#define SEMICOLON 4
#define MAXARG 512              /* 命令行参数个数的最大值 */
#define MAXBUF 512              /* 输入行的最大长度 */
#define FOREGROUND 0
#define BACKGROUND 1



int runcommand(char **cline, int where);
int inarg(char c);
int procline();
int gettok(char *output);
int userin(char *p);
#endif /* _SMALLSH_H */



