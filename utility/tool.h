// -*- C++ -*-

/* @(#)tool.h
 */

#ifndef _TOOL_H
#define _TOOL_H 1

int InterfaceDown(char *cInterfaceName);
int InterfaceUp(char *cInterfaceName);
int SetGateway(const char *cInterfaceName, const char *cGateway);
int setDefGateway(const char * deviceName,const char * defGateway);
int SetIPAddr(char *cInterfaceName, char *IP);
int SetMac(char *cInterfaceName,char *cMacAdd);
int SetNetmask(char *cInterfaceName, char *cNetmask);
int C_Ping(char *IP);
int GetIP(char *cInterfaceName, char *ip);
int InsertDnsToResolv(const char *dns1, const char *dns2);
    
#endif /* _TOOL_H */

