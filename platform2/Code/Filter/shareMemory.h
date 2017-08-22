// -*- C++ -*-

/* @(#)shareMemory.h
 */

#ifndef _SHAREMEMORY_H
#define _SHAREMEMORY_H 1
#include "../../common/message.h"
#include "global.h"


#ifdef __cplusplus
extern "C"
{
#endif
  int initial_shareMemory(int key, struct shared_use_st **shared);
  void *shareMemoryRead(void *args);
  void *shareMemoryWrite(void *args);
#ifdef __cplusplus
}
#endif


#endif /* _SHAREMEMORY_H */

