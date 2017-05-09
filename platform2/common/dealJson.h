// -*- C++ -*-

/* @(#)dealJson.h
 */

#ifndef _DEALJSON_H
#define _DEALJSON_H 1
#ifdef __cplusplus
extern "C"
{
#endif

  extern int getValueFromJson(char *configFile, char *key, char *value);
  extern int setValueToJson(char *configFile, char *key, char *value);
#ifdef __cplusplus
}
#endif

#endif /* _DEALJSON_H */

