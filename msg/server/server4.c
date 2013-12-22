/* 
 *     Created:  05 Jun 2013 14:50:22
 *     Author:   weishijian@UbuntuMac
 Description:  
*/

//#include <config.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>



#define MYPORT 0x8888    // the port users will be connecting to



#define BACKLOG 5     // how many pending connections queue will hold



#define BUF_SIZE 200



int fd_A[BACKLOG];    // accepted connection fd

int conn_amount;    // current connection amount



void showclient()

{

    int i;

    printf("client amount: %d\n", conn_amount);

    for (i = 0; i < BACKLOG; i++) {

        printf("[%d]:%d  ", i, fd_A[i]);

    }

    printf("\n\n");

}



int main(void)

{

    int sock_fd, new_fd;  // listen on sock_fd, new connection on new_fd

    struct sockaddr_in server_addr;    // server address information

    struct sockaddr_in client_addr; // connector's address information

    socklen_t sin_size;

    int yes = 1;

    char buf[BUF_SIZE];

    int ret;

    int i;



    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {

        perror("socket");

        exit(1);

    }



    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {

        perror("setsockopt");

        exit(1);

    }

    

    server_addr.sin_family = AF_INET;         // host byte order

    server_addr.sin_port = htons(MYPORT);     // short, network byte order

    server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP

    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));



    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

        perror("bind");

        exit(1);

    }



    if (listen(sock_fd, BACKLOG) == -1) {

        perror("listen");

        exit(1);

    }



    printf("listen port %d\n", MYPORT);



    fd_set fdsr;

    int maxsock;

    struct timeval tv;



    conn_amount = 0;

    sin_size = sizeof(client_addr);

    maxsock = sock_fd;


    while (1)
    {
	FD_ZERO(&fdsr); //每次进入循环都重建描述符集
	FD_SET(Listen_socket, &fdsr);
	for (i = 0; i < MAXCLIENT; i++) //将存在的套接字加入描述符集
	{
	    if (fd[i] != 0)
	    {
		FD_SET(fd[i], &fdsr);
		if(flag_minutechange == 1)
		{
		    con_time[i]--;
		    if(con_time[i] <= 0)
		    {
			close(fd[i]);
			FD_CLR(fd[i], &fdsr);
			fd[i] = 0;
			conn_amount--;
		    }
		}

	    }
	}
	flag_minutechange = 0;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	ret = select(maxsock + 1, &fdsr, NULL, NULL,&tv); //关键的select()函数，用来探测各套接字的异常
//如果在文件描述符集中有连接请求或发送请求，会作相应处理，
//从而成功的解决了单线程情况下阻塞进程的情况，实现多用户连接与通信

	if (ret < 0) //<0表示探测失败
	{
	    qDebug()<<"failed select\n";
	    break;
	}
	else if (ret == 0) //=0表示超时，下一轮循环
	{
//qDebug()<<"timeout\n";
	    continue;
	}

// 如果select发现有异常，循环判断各活跃连接是否有数据到来
	for (i = 0; i < conn_amount; i++)
	{
	    if (FD_ISSET(fd[i], &fdsr))
	    {
		ret = recv(fd[i], msg, BUF_SIZE, 0);
		if (ret <= 0) // recv<=0,表明客户端关闭连接，服务器也关闭相应连接，并把连接套接子从文件描述符集中清除
		{
		    qDebug("client[%d] close\n", i);
		    close(fd[i]);
		    FD_CLR(fd[i], &fdsr);
		    fd[i] = 0;
		    conn_amount--;
		}
		else //否则表明客户端有数据发送过来，作相应接受处理
		{
		    con_time[i] = MAX_IDLECONNCTIME; //重新写入fd[i]的超时数，再此之后如果MAX_IDLECONNCTIME分钟内此连接无反应，服务器会关闭该连接
		    if (ret < BUF_SIZE)
			emit err_msg_signal("client ip: " + QString::fromLatin1(inet_ntoa(client_addr.sin_addr)) +
					    " port: " + QString::number(ntohs(client_addr.sin_port))+" coming data");
		    qDebug("client[%d] send:%s\n", i, msg);
		    msg[ret] = '\0';
		    emit recv_msg_signal(QString::fromLatin1(msg),fd[i]);
//send(fd[i],msg,ret,0);
		}
	    }
	}


  


// 以下说明异常有来自客户端的连接请求
	if (FD_ISSET(Listen_socket, &fdsr))
	{
	    new_fd = accept(Listen_socket, (struct sockaddr *)&client_addr, &addr_size);
	    if (new_fd <= 0)
	    {
		qDebug("failed accept");
		continue;
	    }

// 判断活跃连接数时候是否小于最大连接数，如果是，添加新连接到文件描述符集中
	    if (conn_amount < MAXCLIENT)
	    {
		for(i = 0;i < MAXCLIENT;i++)
		{
		    if(fd[i] == 0)
		    {

			fd[i] = new_fd;
			con_time[i] = MAX_IDLECONNCTIME; //每次新建立连接，设置该连接的超时数，如果此连接此后MAX_IDLECONNCTIME分钟内无反应，关闭该连接
			break;
		    }

		}
		conn_amount++;
//fd[conn_amount++] = new_fd;
		qDebug("new connection client[%d] %s:%d\n", conn_amount,
		       inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		emit err_msg_signal("client ip: " + QString::fromLatin1(inet_ntoa(client_addr.sin_addr)) +
				    " port: " + QString::number(ntohs(client_addr.sin_port)));
		if (new_fd > maxsock)
		    maxsock = new_fd;
	    }
	    else
	    {
		qDebug("MAXCLIENT arrive, exit\n");
		send(new_fd, "over MAXCLIENT\n", 25, 0);
		close(new_fd);
		continue;
	    }
	}

    }

    exit(0);

}
