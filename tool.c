/*
 * =====================================================================================
 *
 *    Filename:  tool.c
 *
 * Description:  无线版网页使用到的设置ip，网关，mask之类的函数
 *
 *    
 *
 *     Version:  0.1
 *     Created:  30 Apr 2014  09:47:18
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   30 Apr 2014  09:47:18 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 
#include "tool.h"

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>


#include <net/route.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <net/if.h>       /* ifreq struct */
#include <netdb.h>

#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 


/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现



int C_Ping(char *IP)
{
    char command[100] = "";
    sprintf(command, "ping %s -c4", IP);

    int ret = system(command);
    if (WEXITSTATUS( ret ) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    return 1;
}

int GetIP(char *cInterfaceName, char *ip)
{
    int iSocketIoctl;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];

    int num;
    int i;
    if((iSocketIoctl = socket(PF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;

    ioctl(iSocketIoctl, SIOCGIFCONF, &conf);

    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    for(i=0;i < num;i++)
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
        ioctl(iSocketIoctl, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
        {
            if(strcmp(ifr->ifr_name, cInterfaceName) == 0)
            {
                strcpy(ip, inet_ntoa(sin->sin_addr));
                close(iSocketIoctl);
                return -1;

            }
        }
        ifr++;
    }
    close(iSocketIoctl);
    return 0;
}

int SetIPAddr(char *cInterfaceName, char *IP)
{
    int iSocketIoctl;
     
    if((iSocketIoctl = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,cInterfaceName);

    struct sockaddr_in addr;
    bzero(&addr,sizeof(struct sockaddr_in));
    addr.sin_family = PF_INET;
    inet_aton(IP, &addr.sin_addr);

    memcpy(&ifr.ifr_ifru.ifru_addr,&addr,sizeof(struct sockaddr_in));

    if(ioctl(iSocketIoctl, SIOCSIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }
    close(iSocketIoctl);
    return 0;
}


int InterfaceUp(char *cInterfaceName)
{
    int iSocketIoctl;

    if((iSocketIoctl = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, cInterfaceName);

    short flag;
    flag = IFF_UP;
    if(ioctl(iSocketIoctl, SIOCGIFFLAGS, &ifr) < 0)
    {
        perror("ioctl");
        return -1;
    }

    ifr.ifr_ifru.ifru_flags |= flag;

    if(ioctl(iSocketIoctl, SIOCSIFFLAGS, &ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }
    close(iSocketIoctl);
    return 0;

}


int InterfaceDown(char *cInterfaceName)
{
    if(strcmp(cInterfaceName,"lo") == 0)
    {
        printf("You can't pull down interface lo/n");
        return 0;
    }

    int iSocketIoctl;

    if((iSocketIoctl = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, cInterfaceName);
    
    short flag;
    flag = ~IFF_UP;
    if(ioctl(iSocketIoctl, SIOCGIFFLAGS, &ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }

    ifr.ifr_ifru.ifru_flags &= flag;

    if(ioctl(iSocketIoctl, SIOCSIFFLAGS, &ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }
    close(iSocketIoctl);
    return 0;
}


int SetMac(char *cInterfaceName,char *cMacAdd)
{
    int iSocketIoctl;
    if((iSocketIoctl = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket");
        return -1;
    }
   
    struct ifreq ifr;
    strcpy(ifr.ifr_name, cInterfaceName);

    if(ioctl(iSocketIoctl,SIOCGIFHWADDR,&ifr) < 0)
    {
        perror("ioctl");
        return -1;
    }

    sa_family_t get_family;
    get_family = ifr.ifr_ifru.ifru_hwaddr.sa_family;
   
    InterfaceDown(cInterfaceName);

    bzero(&ifr,sizeof(struct ifreq));
    strcpy(ifr.ifr_name,cInterfaceName);
    ifr.ifr_ifru.ifru_hwaddr.sa_family = get_family;

    short tmp;
    int i,j;
    j = 0;
    for(i = 0;i < 17; i += 3)
    {
        if(cMacAdd[i] < 58 && cMacAdd[i] > 47)
            tmp = cMacAdd[i]-48;
        if(cMacAdd[i] < 71 && cMacAdd[i] > 64)
            tmp = cMacAdd[i]-55;
        if(cMacAdd[i] < 103 && cMacAdd[i] > 96)
            tmp = cMacAdd[i]-87;
        tmp = tmp << 4;
        if(cMacAdd[i+1] < 58 && cMacAdd[i+1] > 47)
            tmp |= (cMacAdd[i+1]-48);
        if(cMacAdd[i+1] < 71 && cMacAdd[i+1] > 64)
            tmp |= (cMacAdd[i+1]-55);
        if(cMacAdd[i+1] < 103 && cMacAdd[i+1] > 96)
            tmp |= (cMacAdd[i+1]-87);
        memcpy(&ifr.ifr_ifru.ifru_hwaddr.sa_data[j++],&tmp,1);
    }

    if(ioctl(iSocketIoctl, SIOCSIFHWADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }

    InterfaceUp(cInterfaceName);
    close(iSocketIoctl);
    return 0;
}



int SetNetmask(char *cInterfaceName, char *cNetmask)
{
    int iSocketIoctl;

    if((iSocketIoctl = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket");
        return -1;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, cInterfaceName);

    struct sockaddr_in netmask_addr;
    bzero(&netmask_addr,sizeof(struct sockaddr_in));
    netmask_addr.sin_family = PF_INET;
    inet_aton(cNetmask,&netmask_addr.sin_addr);

    memcpy(&ifr.ifr_ifru.ifru_netmask,&netmask_addr,sizeof(struct sockaddr_in));

    if(ioctl(iSocketIoctl,SIOCSIFNETMASK,&ifr) < 0)
    {
        perror("ioctl");
        close(iSocketIoctl);
        return -1;
    }
    close(iSocketIoctl);
    return 0;
}


int SetGateway(const char *cInterfaceName, const char *cGateway)
{
    int iSocketIoctl;
    char cRtDev[10] = {0};
    strcpy(cRtDev, cInterfaceName);
    iSocketIoctl = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSocketIoctl < 0)
    {
        perror("socket");
        return -2;
    }
    struct sockaddr_in addr;
    struct rtentry route;  /* route item struct */

    memset(&route, 0, sizeof(route));


    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(cGateway);
    addr.sin_port = 0;

    (( struct sockaddr_in*)&route.rt_dst)->sin_family = AF_INET;
    (( struct sockaddr_in*)&route.rt_dst)->sin_addr.s_addr = 0;
    (( struct sockaddr_in*)&route.rt_dst)->sin_port = 0;

    (( struct sockaddr_in*)&route.rt_genmask)->sin_family = AF_INET;
    (( struct sockaddr_in*)&route.rt_genmask)->sin_addr.s_addr = 0;
    (( struct sockaddr_in*)&route.rt_genmask)->sin_port = 0;

    memcpy((void *) &route.rt_gateway, &addr, sizeof(addr));
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_dev = cRtDev;

    if(ioctl(iSocketIoctl, SIOCADDRT, &route) < 0)
    {
        perror("SIOCADDRT");
        close(iSocketIoctl);
        return -1;
    }
    close(iSocketIoctl);
    return 0;
}



int setDefGateway(const char * deviceName,const char * defGateway)
{
    int sockfd;
    struct rtentry rm;
    struct sockaddr_in ic_gateway ;// Gateway IP address
    int err;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("socket is -1\n");
        return -1;
    }

    memset(&rm, 0, sizeof(rm));


    ic_gateway.sin_family = AF_INET;
    ic_gateway.sin_addr.s_addr = inet_addr(defGateway);
    ic_gateway.sin_port = 0;

    (( struct sockaddr_in*)&rm.rt_dst)->sin_family = AF_INET;
    (( struct sockaddr_in*)&rm.rt_dst)->sin_addr.s_addr = 0;
    (( struct sockaddr_in*)&rm.rt_dst)->sin_port = 0;

    (( struct sockaddr_in*)&rm.rt_genmask)->sin_family = AF_INET;
    (( struct sockaddr_in*)&rm.rt_genmask)->sin_addr.s_addr = 0;
    (( struct sockaddr_in*)&rm.rt_genmask)->sin_port = 0;

    memcpy((void *) &rm.rt_gateway, &ic_gateway, sizeof(ic_gateway));
    rm.rt_flags = RTF_UP | RTF_GATEWAY;
    if ((err = ioctl(sockfd, SIOCADDRT, &rm)) < 0)
    {
        printf("SIOCADDRT failed , ret->%d\n",err);
        return -1;
    }
    return 0;
}



int InsertDnsToResolv(const char *dns1, const char *dns2)
{


    if (access("/etc/resolv.conf",0) == 0)
    {
        remove("/etc/resolv.conf");
    }
    else
    {
    }

    FILE *file = fopen("/etc/resolv.conf", "w");
    rewind(file);
    fprintf(file, "%s\n", dns1);
    fprintf(file, "%s\n", dns2);
    fclose(file);
    return 0;
}
