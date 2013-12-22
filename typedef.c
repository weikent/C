/* 
 *     Created:  08 10月 2013 13:17:56
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */
#include <stdio.h>
#include <signal.h>



typedef void (*HANDLER)(int);
HANDLER signal(int, HANDLER);


void temp1(int a)
{
    printf ("aaaaaaaaaaaa %d \n", a);
}

int main(int argc, char *argv[])
{
    HANDLER a = temp1;
    signal(SIGINT, *temp1);
    printf ("%-50s_%-50s\n","a", "b");
    for ( ;  ;  )
    {}
    return 0;
}

