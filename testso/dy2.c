//#include <config.h>
/* 
 *     Created:  12  8月 2013 16:29:46
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdio.h>
#include <dlfcn.h>


#include "datetime.h"


int main(int argc, char *argv[])
{
    DATETYPE d;
    TIMETYPE t;
    void *dp;
    char *error;
    
    puts("动态链接库应用范例");



    getdate(&d);
    printf ("当前日期：%04d - %02d - %02d\n", d.year, d.mon, d.day);



    
    gettime(&t);
    printf ("当前时间：%02d:%02d:%02d\n",t.hour, t.min, t.sec);


    
    return 0;
}






