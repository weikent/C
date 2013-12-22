/* 
 *     Created:  24  7月 2013 14:32:25
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>


/* get MAC address */

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include <sys/ioctl.h>

#include <sys/socket.h>

#include <sys/errno.h>

#include <linux/if.h>

 

int main(int argc, char **argv)

{

    int i, sockfd;
    static struct ifreq req;
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Unable to create socket: ");
        exit(1);
    }
    strcpy(req.ifr_name, "eth0");
    ioctl(sockfd, SIOCGIFHWADDR, &req);
    printf("MAC address of %s:	",req.ifr_name);
    for (i = 0; i < 5; i++) {
        printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF);
        printf(":");
    }
    printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF);
    close(sockfd);
    return 0;
}

