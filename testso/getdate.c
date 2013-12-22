/* 
 *     Created:  12  8月 2013 16:20:09
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <time.h>
#include "datetime.h"

int getdate(DATETYPE *d)
{
    long ti;
    struct tm *tm;
    
    time(&ti);
    tm = localtime(&ti);
    d->year = tm->tm_year + 1900;
    d->mon = tm->tm_mon + 1;
    d->day = tm->tm_mday;
}


