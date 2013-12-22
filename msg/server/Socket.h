#include <sys/socket.h>


int Accept(int sockfd, struct sockaddr * sockaddr, int *len);
int Bind(int sockfd, const struct sockaddr * sockaddr, int len);
int Connect(int sockfd, const struct sockaddr * sockaddr, int len);
int Listen(int sockfd, int num);
int Socket(int family, int type, int protocol);


void Send(int sockfd, const void *ptr, size_t nbytes, int flags);
ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags);

