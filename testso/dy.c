/* 
 *     Created:  12  8月 2013 16:29:46
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdio.h>
#include <dlfcn.h>

#define SOFILE "./my.so"

#define SHARED
#include "datetime.h"


int main(int argc, char *argv[])
{
    DATETYPE d;
    TIMETYPE t;
    void *dp;
    char *error;
    
    puts("动态链接库应用范例");

    dp = dlopen(SOFILE, RTLD_LAZY);

    if (dp == NULL)
    {
	fputs(dlerror(), stderr);
	exit(1);
    }


    getdate = dlsym(dp, "getdate");
    
    error = dlerror();
    if (error)
    {
	fputs(error, stderr);
	exit(1);
    }

    getdate(&d);
    printf ("当前日期：%04d - %02d - %02d\n", d.year, d.mon, d.day);


    gettime = dlsym(dp, "gettime");
    error = dlerror();
    if (error)
    {
	fputs(error, stderr);
	exit(1);
    }
    
    gettime(&t);
    printf ("当前时间：%02d:%02d:%02d\n",t.hour, t.min, t.sec);


    dlclose(dp);
    
    return 0;
}



