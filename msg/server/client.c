/*************************************
文件名： client.c 
linux 下socket网络编程简例  - 客户端程序
服务器端口设为 0x8888   （端口和地址可根据实际情况更改，或者使用参数传入）
服务器地址设为 192.168.1.104
作者:kikilizhm#163.com (将#换为@)
*/

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <string.h>

int main()
{

    int cfd; /* 文件描述符 */
    int recbytes;
    int sin_size;
    char buffer[1024]={0};    /* 接受缓冲区 */
    struct sockaddr_in s_add,c_add; /* 存储服务端和本端的ip、端口等信息结构体 */
    unsigned short portnum=0;  /* 服务端使用的通信端口，可以更改，需和服务端相同 */

    int socketTOServer = -1;
    char serverIP[16] = {0};
    int serverPort = 0;

    printf("Hello,welcome to client !\r\n");



			 
    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);

    if (socketTOServer == -1)
    {

    }



    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
    s_add.sin_port=htons(12345); 

    int connectTimes = 0;

    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
	//perror("connect fail!");
	printf("connect fail");
    }
    
    printf("sdfsfsf");
    return 0;



}
