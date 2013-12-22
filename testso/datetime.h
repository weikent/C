// -*- C++ -*-

/* @(#)datetime.h
 */

#ifndef _DATETIME_H
#define _DATETIME_H 1

typedef struct
{
    int year;
    int mon;
    int day;
} DATETYPE;

typedef struct
{
    char hour;
    char min;
    char sec;
} TIMETYPE;

#ifdef SHARED
int (*getdate)(DATETYPE *d);
#else
int getdate(DATETYPE *d);
#endif

#ifdef SHARED
int (*gettime)(TIMETYPE *d);
#else
int gettime (TIMETYPE *d);
#endif

#endif /* _DATETIME_H */

