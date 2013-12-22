// -*- C++ -*-

/* @(#)CLib.h
 */

#ifndef _CLIB_H
#define _CLIB_H 1

typedef struct CStashTag
{
    int size;
    int quantity;
    int next;
    
    unsigned char *storage;
} CStash;

void initialize(CStash *s, int size);
void cleanup(CStash *s);
int add(CStash *s, const void *element);
void *fetch(CStash *s, int index);
int count(CStash *s);
void inflate(CStash *s, int increase);

#endif /* _CLIB_H */

