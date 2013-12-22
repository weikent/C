/* 
 *     Created:  05 Jun 2013 15:04:55
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






void run()
{
    char msg[BUF_SIZE];
    int Listen_socket,ret,on;
    struct sockaddr_in local_addr;
    struct sockaddr_in client_addr;
    int i;
    fd_set fdsr; //文件描述符集的定义
    socklen_t addr_size;
    addr_size = sizeof(struct sockaddr_in);

    int conn_amount = 0; //当前最大活跃连接数
    int new_fd;
    struct timeval tv;

//建立socket套接字
    if( (Listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
	emit err_msg_signal("failed create socket");
    }

//bind API 函数将允许地址的立即重用
    on = 1;
    ret = setsockopt( Listen_socket, SOL_SOCKET, SO_REUSEADDR,
		      &on, sizeof(on) );

    int nNetTimeout=2000;//2秒
//设置发送时限
    setsockopt(Listen_socket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int) );
//设置接收时限
    setsockopt(Listen_socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

//设置本机服务类型
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

//while(flag_port == 0)
//绑定本机IP和端口号
    if(bind(Listen_socket, (struct sockaddr*)&local_addr, sizeof(struct sockaddr)) == -1)
    {
	emit err_msg_signal("failed bind");
    }

//监听客户端连接
    if(listen(Listen_socket, 8) == -1)
    {
	emit err_msg_signal("failed listen");
    }


    QTime current_time;
    current_time = QTime::currentTime();
    int flag_minutechange = 0,lastminute = current_time.currentTime().minute();
    int maxsock = Listen_socket;

  


/***************************************
以下为并发连接处理，系统关键部分
***************************************/

    while (1)
    {

	if( current_time.currentTime().minute() != lastminute) //每次循环开始都读取系统时间，与上次分钟数比较，为以下超时判断提供依据
	{
	    lastminute = current_time.currentTime().minute();
	    flag_minutechange = 1;
	}

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
}

int main(int argc, char *argv[])
{
    run();
    return 0;
}
