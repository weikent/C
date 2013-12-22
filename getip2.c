/* 
 *     Created:  26  7月 2013 14:21:11
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/if.h>

int main(void)
{

    char *name = "eth0";
    int s;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUFSIZ];
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
	    if (strcmp(ifr->ifr_name, name) == 0)
	    {
		printf("%s (%s)\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
	    }
            printf("%s (%s)\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
        }
        ifr++;
    }
}
