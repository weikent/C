#include "Socket.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/in.h>
#include <string.h>

int Accept(int sockfd, struct sockaddr *sockaddr, int *len)
{
	int n;
	if ((n = accept(sockfd,sockaddr, len)) < 0)
	{
		printf ("accept error\n");
	}
	return n;
}

int Bind(int sockfd, const struct sockaddr * sockaddr, int len)
{
	int n;
	if ((n = bind(sockfd, sockaddr, len)) < 0)
	{
		printf ("bind error\n");
	}
	return n;
}

int Connect(int sockfd, const struct sockaddr * sockaddr, int len)
{
	int n;
	if ((n = connect(sockfd, sockaddr, len)) < 0)
	{
		printf ("connect error\n");
	}
	return n;
}

int Listen(int sockfd, int num)
{
	int n ;
	if ((n = listen(sockfd, num)) < 0)
	{
		printf ("listen error\n");
	}
	return n;
}

int Socket(int family, int type, int protocol)
{
	int n;
	if ((n = socket(family, type, protocol)) < 0)
	{
		printf ("socket error\n");
		exit(1);
	}
	return n;
}

void Send(int sockfd, const void *ptr, size_t nbytes, int flags)
{
	if (send(sockfd, ptr, nbytes, flags) != nbytes)
	{
		printf ("Send error\n");
	}
}

ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags)
{
	ssize_t		n;

	if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
	{
		printf ("Receive error\n");
	}
	return(n);
}
