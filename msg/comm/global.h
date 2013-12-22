// -*- C++ -*-

/* @(#)global.h
 */

#ifndef _GLOBAL_H
#define _GLOBAL_H 1


enum deviceModel {AP, CLIENT};

extern deviceModel deviceModel;


struct PairInfo {
    char sys_id[8];
    char op_code[8];
    long int timeOut;
    char ssid[128];
    char password[128];
    
    int dhcp_status;
    char ip_addr[15];
    char mask[15];
    char gete_way[15];
    char dns1[15];
    char dns2[15];
    char server_ip[15];
    int server_port;
};

struct WiFiInfo{
    char ssid[128];
    char rssi;
    char encryption[10];
};
    
    
#endif /* _GLOBAL_H */

