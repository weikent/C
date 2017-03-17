// -*- C++ -*-

/* @(#)globalStruct.h
 */

#ifndef _GLOBALSTRUCT_H
#define _GLOBALSTRUCT_H 1




struct WiFiInfo
{
    char ssid[LEN_OF_WIFIINFO];
    char rssi;
    char encryption[10];
};


struct TimeSyncInfo
{
    char sys_id[8];
    char op_code[8];
    long int timeOut;

    char verifiyCode[LEN_OF_VERIFIYCODE];
    char nowTime[20];
    char uuid[60];
};
    
struct PairInfo 
{
    char sys_id[8];
    char op_code[8];
    long int timeOut;
    char ssid[LEN_OF_WIFIINFO];
    char password[LEN_OF_WIFIINFO];
    
    int dhcp_status;
    char ip_addr[LEN_OF_IPADDRESS];
    char mask[LEN_OF_IPADDRESS];
    char gate_way[LEN_OF_IPADDRESS];
    char dns1[LEN_OF_IPADDRESS];
    char dns2[LEN_OF_IPADDRESS];
    char server_ip[LEN_OF_IPADDRESS];
    int server_port;
};


#endif /* _GLOBALSTRUCT_H */

