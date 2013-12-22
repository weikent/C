/* 
 *     Created:  16  8月 2013 11:14:30
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>



#define MCAST_PORT 8888;
#define MCAST_ADDR "224.0.0.88"/    /*一个局部连接多播地址，路由器不进行转发*/
#define MCAST_DATA "BROADCAST TEST DATA"            /*多播发送的数据*
#define MCAST_INTERVAL 5                            /*发送间隔时间*/
int main(int argc, char*argv)
{
    int s;
    struct sockaddr_in mcast_addr;     
    s = socket(AF_INET, SOCK_DGRAM, 0);         /*建立套接字*/
    if (s == -1)
    {
        perror("socket()");
        return -1;
    }
   
    memset(&mcast_addr, 0, sizeof(mcast_addr));/*初始化IP多播地址为0*/
    mcast_addr.sin_family = AF_INET;                /*设置协议族类行为AF*/
    mcast_addr.sin_addr.s_addr = inet_addr("224.0.0.88");   /*设置多播IP地址*/
    mcast_addr.sin_port = 0x7777;//htons(MCAST_PORT);        /*设置多播端口*/
   
                                                    /*向多播地址发送数据*/
    while(1) {
        int n = sendto(s,                           /*套接字描述符*/
                                    MCAST_DATA,     /*数据*/
                                    sizeof(MCAST_DATA),    /*长度*/
                                    0,
                                    (struct sockaddr*)&mcast_addr,
                                    sizeof(mcast_addr)) ;
        if( n < 0)
        {
            perror("sendto()");
            return -2;
        }      
       
        sleep(20);                          /*等待一段时间*/
    }
    return 0;
}

