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
    unsigned short portnum=0x8888;  /* 服务端使用的通信端口，可以更改，需和服务端相同 */



    printf("Hello,welcome to client !\r\n");
/* 建立socket 使用因特网，TCP流传输 */
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
	printf("socket fail ! \r\n");
	return -1;
    }
    printf("socket ok !\r\n");
/* 构造服务器端的ip和端口信息，具体结构体可以查资料 */
    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
//	s_add.sin_addr.s_addr= inet_addr("127.0.0.1"); /* ip转换为4字节整形，使用时需要根据服务端ip进行更改 */
//    inet_aton("127.0.0.1", s_add.sin_addr.s_addr);
//    inet_pton(AF_INET, "192.168.1.234", &s_add.sin_addr);
    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
    s_add.sin_port=htons(portnum); /* 这里htons是将short型数据字节序由主机型转换为网络型，其实就是
				      将2字节数据的前后两个字节倒换，和对应的ntohs效果、实质相同，只不过名字不同。htonl和ntohl是
				      操作的4字节整形。将0x12345678变为0x78563412，名字不同，内容两两相同，一般情况下网络为大端，
				      PPC的cpu为大端，x86的cpu为小端，arm的可以配置大小端，需要保证接收时字节序正确。
				   */

    printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port); /* 这里打印出的是小端
										    和我们平时看到的是相反的。 */

/* 客户端连接服务器，参数依次为socket文件描述符，地址信息，地址结构大小 */
    if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
	printf("connect fail !\r\n");
	return -1;
    }
    printf("connect ok !\r\n");
/*连接成功,从服务端接收字符*/

//    char text[] = "~0042{\"optCode\":\"heart\",\"devID\":\"34bca6010203\"}";
    char text[] = "~0497{\"sysID\":\"0150\",\"seqOfIns\":\"161\",\"numOfDev\":\"1\",\"optCode\":\"0030\",\"devArray\":[{\"devID\":\"34bca6000000\",\"numOfSen\":\"2\",\"senArray\":[{\"senID\":\"0\",\"senType\":\"0005\",\"numOfParam\":\"3\",\"paramArray\":[{\"paramType\":\"0055\",\"paramValue\":\"nan\"},{\"paramType\":\"0060\",\"paramValue\":\"nan\"},{\"paramType\":\"0065\",\"paramValue\":\"nan\"}]},{\"senID\":\"1\",\"senType\":\"0005\",\"numOfParam\":\"3\",\"paramArray\":[{\"paramType\":\"0055\",\"paramValue\":\"nan\"},{\"paramType\":\"0060\",\"paramValue\":\"nan\"},{\"paramType\":\"0065\",\"paramValue\":\"nan\"}]}]}]}";
    
//    char text[] = "{\"sysID\":\"0010\",\"optCode\":\"0020\",\"timeOut\":\"111111111\",\"SSID\":\"startech-218\",\"password\":\"startech1\",\"DHCPStatus\":\"1\",\"IPAdress\":\"0.0.0.0\",\"mask\":\"0.0.0.0\",\"gateway\":\"0.0.0.0\",\"DNS1\":\"0.0.0.0\",\"DNS2\":\"0.0.0.0\",\"serverIP\":\"192.168.5.74\",\"serverPort\":\"14567\"}";
    if (write(cfd, text, sizeof(text)) == -1)
    {
	printf ("write error 4\n");
    }
    printf("write ok\n");
    for ( ;  ;  )
    {
    	if(-1 == (recbytes = read(cfd,buffer,1024)))
    	{
    	    printf("read data fail !\r\n");
    	    return -1;
    	}
//    	printf("read ok\r\nREC:\r\n");

	if (recbytes > 1)
	{
    	buffer[recbytes]='\0';
    	printf("%s\r\n",buffer);
	    
	}

    }

    getchar(); /* 此句为使程序暂停在此处，可以使用netstat查看当前的连接 */
    close(cfd); /* 关闭连接，本次通信完成 */
    return 0;



}

