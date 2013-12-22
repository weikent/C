/* 
 *     Created:  15 May 2013 14:45:57
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
#include "getOtherID.h"
#include "msgqueue.h"

#include "global/global.h"
#include "dataParse.h"


#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <csignal>
#include <signal.h>

#include <stdbool.h>
//#include <iostream>
//#include <iostream.h>


#include <sys/socket.h>
#include <sys/types.h>
//#include <linux/in.h>
#include <arpa/inet.h>

#include <string.h>

//using namespace std;

#define MAX_DATA_SEND_TO_SERVER 1024
#define HEARTINTERVAL 2



//msgqueue msg;

int message_t;  //消息队列
int localID;   //本地进程ID
struct mymsgbuf qbuf;   // 消息队列的结构体，用来保存消息队列中的信息。
int connectModel = -1;   // 0: 短连接    1:长连接   -1:程序首次启动
int sameLAN = 0;    //0:不在同一个LAN   1:在同一个LAN
bool hasDataToSend = false; // 本地层是不是有信息要发到服务器。
pthread_t thread[3];  // thread1: getotherID,  thread2: send data to server   thread3: the same LAN, start this thread to create a socket server.
//char dataForServer[MAX_DATA_SEND_TO_SERVER];  //
int socketTOServer;  //与服务器连接的socket描述符。



/**************************/
/* 解析从服务器获取的数据 */
/**************************/
int serverDataAnalyze(const char *data)
{
    
    return 0;
}

/**************************************/
/* 服务器数据重新打包成本地层可用数据 */
/**************************************/
int serverDataRePackage(const char *data, char *newData)
{
    return 0;
}

/**************************/
/* 解析从本地层获取的数据 */
/**************************/
int localDataAnalyze(const char *data)
{
    return 0;
}

/********************************************/
/* 本地层数据重新打包成服务器可以解析的数据 */
/********************************************/
int localDataRePackage(const char *data, char *newData)
{
    return 0;
}


/********************************************************/
/* 检查从服务器接收的数据，是不是要改变程序的连接模式。 */
/* 即设置长/短连接				        */
/********************************************************/
int changeConnetModel(char *buff)
{
    char temp[10];
    bzero(temp, sizeof(temp));
    strncpy(temp, buff, 6);
    printf ("%s\n",temp);
    if (strcmp(temp, "model0") == 0)
    {
	printf ("change connect mode to short connection!\n", 0);
	close(socketTOServer);
	return 0;
    }
    else if ((strcmp(temp, "model1") == 0) || (strcmp(temp, "heart") == 0))
    {
	printf ("change connect mode to long connection!\n", 1);
	return 1;
    }
    else
    {
	close(socketTOServer);
	printf ("change connect mode to short connection!\n", 0);
	return 0;
    }
}


/*****************************/
/* 检查是否与手机在同一个LAN */
/*****************************/
int checkIsTheSameLAN(char *buff)
{
    char temp[10];
    bzero(temp, sizeof(temp));
    strncpy(temp, buff, 4);
    if (strcmp(temp, "same") == 0)
    {
	//same LAN
	return 1;
    }
    else
    {
	// not the same LAN
        // close thread.
	return 0;
    }
    return 0;
}

/**************************/
/* 与服务器连接的主要函数 */
/**************************/
void *connectServer(void *arg)
{

    int recbytes;
    int sin_size;
    char buffer[MAX_DATA_SEND_TO_SERVER]={0};    /* 接受缓冲区 */

    int ret;
    unsigned short portnum=0x8888;  /* 服务端使用的通信端口，可以更改，需和服务端相同 */


    printf ("%s\n",*(char**)arg);


    for ( ;  ;  )
    {

	if (connectModel == -1)
	{
	    //一次socket 连接到server, 通知server
	    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketTOServer == -1)
	    {
		//send message to local.
	    }

	    struct sockaddr_in s_add; 

	    bzero(&s_add,sizeof(struct sockaddr_in));
	    s_add.sin_family=AF_INET;
	    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
	    s_add.sin_port=htons(portnum); 

	    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	    {
		printf("connect fail !\r\n");

	    }

	    if (write(socketTOServer, "connect", 7) == -1)
	    {
		printf ("write error\n");

	    }
	    for ( ;  ;  )
	    {
		memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
		if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
		{
		    printf("read data fail !\n");
		}
		printf("read ok\nREC:\n");

		buffer[recbytes]='\0';
		printf("%s\n",buffer);
		break;
	    }

	    connectModel = changeConnetModel(buffer);

	    printf ("close connect\n");
	    close(socketTOServer);

	}
	if (connectModel == 0)//short socket connection
	{
	    if (hasDataToSend)
	    {
		//send data to server
		printf ("has Data to send!\n");

		socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
		if (socketTOServer == -1)
		{
		    //send message to local.
		}

		struct sockaddr_in s_add; 

		bzero(&s_add,sizeof(struct sockaddr_in));
		s_add.sin_family=AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
		s_add.sin_port=htons(portnum); 

		if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
		{
		    printf("connect fail !\r\n");

		}

		printf ("%s\n",qbuf.mtext);
		if (write(socketTOServer, qbuf.mtext, sizeof(qbuf.mtext)) == -1)
		{
		    printf ("write error\n");

		}
		for ( ;  ;  )
		{
		    memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
		    if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
		    {
			printf("read data fail !\n");
		    }
		    printf("read ok\nREC:\n");

		    buffer[recbytes]='\0';
		    printf("%s\n",buffer);
		    break;
		}


		printf ("close this short connect\n");
		close(socketTOServer);
		connectModel = changeConnetModel(buffer);

		hasDataToSend = false;

	    }
	    else
	    {
		//do nothing.
	    }
	}

	if (connectModel == 1)// long socket connection
	{

	    //send data to server, Although local app not send data, the comm app need send "heart" to server.
	    printf ("has Data to send!\n");

	    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketTOServer == -1)
	    {
		//create socket failed.
		//send message to local.
	    }

	    struct sockaddr_in s_add; 

	    bzero(&s_add,sizeof(struct sockaddr_in));
	    s_add.sin_family=AF_INET;
	    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
	    s_add.sin_port=htons(portnum); 

	    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	    {
		printf("connect fail !\r\n");

	    }

	    // use for when the first time change to long connection, send a "heart" to server.
	    long int li = HEARTINTERVAL;

	    fd_set fdsr;
	    struct timeval tv;
	    int maxsock;
	    maxsock = socketTOServer;

	    while(connectModel == 1)
	    {
		
		FD_ZERO(&fdsr);

		FD_SET(socketTOServer, &fdsr);

		// timeout setting
		tv.tv_sec = 5;
		tv.tv_usec = 0;


		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);

		if (ret < 0) {
		    perror("select");
		    break;
		} else if (ret == 0) {
		    printf("timeout\n");
//		    continue;
		}



		if (FD_ISSET(socketTOServer, &fdsr))
		{

		    ret = read(socketTOServer, buffer, MAX_DATA_SEND_TO_SERVER);

		    if (ret <= 0)
		    {        // server close this socket connect.

			printf("server close\n");

			close(socketTOServer);
			connectModel = 0;

		    } 
		    else
		    {        // receive data from server

			if (ret < MAX_DATA_SEND_TO_SERVER)
			{
			    memset(&buffer[ret], '\0', 1);
			}
			printf("%s\n", buffer);
//			send(fd_A[i], buf, sizeof(buf), 0);
		    }
		}


		if (hasDataToSend && (connectModel == 1))
		{

		    //send data to server
		    printf ("%s\n",qbuf.mtext);
		    if (write(socketTOServer, qbuf.mtext, sizeof(qbuf.mtext)) == -1)
		    {
			printf ("write error\n");

		    }
		    for ( ;  ;  )
		    {
			memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
			if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
			{
			    printf("read data fail !\n");
			}
			printf("read ok\nREC:\n");

			buffer[recbytes]='\0';
			printf("%s\n",buffer);
			connectModel = changeConnetModel(buffer);
			break;
		    }

		    hasDataToSend = false;
		}
		else if (connectModel == 1)
		{
		    /* sleep(5); */
		    //心跳数据
		    if (li == HEARTINTERVAL)
		    {
			li = 0;
			if (write(socketTOServer, "heart", 5) == -1)
			{
			    printf ("write error\n");

			}

			memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
			if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
			{
			    printf("read data fail !\n");
			}
			printf("read ok\nREC:\n");

			buffer[recbytes]='\0';
			printf("%s\n",buffer);
			connectModel = changeConnetModel(buffer);			    
			printf ("one connect finish-----------------------------\n");
		    }
		    li++;
		}
	    }
	}
    }
}

void localData(int i)
{
    read_message(message_t, (struct mymsgbuf *) &qbuf, 2);
    hasDataToSend = true;
}

void clear(int i)
{
    printf ("clear\n");
    remove_queue(message_t);
    exit(0);
}

void *getlocalID(void *arg)
{
//    getOtherID otherID;
    
    const char * local = "local";
    for (; ; )
    {
	localID = getID(local);
//	printf ("%d\n",localID);
	sleep(1);
    }
}


int main(int argc,char ** argv)
{
    struct mymsgbuf qbuf;
    /* char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhh\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actID\":\"0\",\"actValue\":\"0\"}]}]}]}"; */
    /* serverDataParse(text6); */
    message_t = create_queue();

    printf ("message_t = %d\n",message_t);

    int temp; 
    memset(&thread, 0, sizeof(thread));

    if((temp = pthread_create(&thread[0], NULL, getlocalID, NULL)) != 0)
	printf("获取ID线程创建失败!\n"); 
    else 
	printf("获取ID线程被创建\n"); 


    char *aa = argv[2];
    printf ("%s\n",aa);

    if ((temp = pthread_create(&thread[1], NULL, connectServer, &aa)) != 0)
    {
	printf ("failed\n");
    }
    else
    {
	printf ("ok\n");
    }
	    

    signal(SIGUSR1, localData);
    signal(SIGINT, clear);
    while(1)
    {

//注释此些行，不用这些代码来发数据给local，而是等server发来数据之后，再发给local。
// 	if (localID > 0)
// 	{
// 	    char cmd[256];
// 	    memset(cmd, 0, 256);
// 	    printf ("please input a msessage(send to local):");
// 	    fgets(cmd, 255, stdin);
// 	    msg.send_message(message_t, (struct mymsgbuf *) &qbuf, 1, cmd);
// 	    kill(localID, SIGUSR1);
// //	    sleep(3);
// 	}
    }
}

