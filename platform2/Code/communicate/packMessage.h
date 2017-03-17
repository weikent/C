// -*- C++ -*-

/* @(#)packMessage.h
 */

#ifndef _PACKMESSAGE_H
#define _PACKMESSAGE_H 1
#include "global.h"


int packMessage(struct mqttSend *mqttSend, unsigned char **packedMessage);

#endif /* _PACKMESSAGE_H */

