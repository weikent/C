/* 
 *     Created:  24  7月 2013 15:42:28
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include "utility.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <linux/if.h>
#include <sys/time.h>    
#include <sstream>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <netinet/in.h>


using namespace std;

utility::utility()
{
}

utility::~utility()
{
}


int utility::getMAC(char *name, string &mac)
{
    int i, sockfd;

    static struct ifreq req;
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Unable to create socket: ");
        exit(1);
    }


    strcpy(req.ifr_name, name);
    ioctl(sockfd, SIOCGIFHWADDR, &req);
    printf("MAC address of %s:	",req.ifr_name);
    char tempMac[12];
    /* printf("%02X%02X%02X%02X%02X%02X", req.ifr_hwaddr.sa_data[0]&0xFF, req.ifr_hwaddr.sa_data[1]&0xFF, req.ifr_hwaddr.sa_data[2]&0xFF, req.ifr_hwaddr.sa_data[3]&0xFF, req.ifr_hwaddr.sa_data[4]&0xFF, req.ifr_hwaddr.sa_data[5]&0xFF); */ 
    sprintf(tempMac, "%02X%02X%02X%02X%02X%02X", req.ifr_hwaddr.sa_data[0]&0xFF, req.ifr_hwaddr.sa_data[1]&0xFF, req.ifr_hwaddr.sa_data[2]&0xFF, req.ifr_hwaddr.sa_data[3]&0xFF, req.ifr_hwaddr.sa_data[4]&0xFF, req.ifr_hwaddr.sa_data[5]&0xFF);

    mac = tempMac;
    /* for (i = 0; i < 5; i++) { */
    /*     printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF); */
    /*     printf(":"); */
    /* } */
    /* printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF); */
    close(sockfd);
    return 0;
}


string utility::getIP(char *name)
{
      int s;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
    char *ip;
    int num;
    int i;
    s = socket(PF_INET, SOCK_DGRAM, 0);
    conf.ifc_len = BUFSIZ;
    conf.ifc_buf = buff;
    ioctl(s, SIOCGIFCONF, &conf);
    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;
    for(i=0;i < num;i++)
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
        ioctl(s, SIOCGIFFLAGS, ifr);
        if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
        {
		if(strcmp(ifr->ifr_name, name) == 0)
		{
			return inet_ntoa(sin->sin_addr);
		}
            printf("%s (%s)\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
        }
        ifr++;
    }
	return "";
}

string utility::getCurrentTime()
{  
    struct timeval tv;  
    gettimeofday(&tv,NULL);  
    long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;  
    stringstream ss;
    ss << time;
    return ss.str();
}  


string& utility::trim(string &str, string oldstr,string::size_type pos)
{
    string delim = oldstr;
    pos = str.find_first_of(delim, pos);
    if (pos == string::npos)
        return str;
    return trim(str.erase(pos, 1), delim);
}

