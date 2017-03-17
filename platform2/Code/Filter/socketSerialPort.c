/**
 *   \file SocketSerialPort.c
 *   \brief 串口通讯 文件
 *
 *  串口通讯的相关函数都放在这个文件。
 *  put all functions about serial communication into this file.
 *
 *
 * Thu Mar  9 16:35:38 2017
 *
 * author: weishijian
 *
 */

#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
#include <termios.h>//终端控制定义
#include <errno.h>
#include <strings.h>
#include "global.h"



 int serial_fd = 0;

int baudRete = 57600;


//打开串口并初始化设置
int init_serial(void)
{
  serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
  if (serial_fd < 0) {
    perror("open");
    return -1;
  }

  //串口主要设置结构体termios <termios.h>
  struct termios options;

  /**1. tcgetattr函数用于获取与终端相关的参数。
   *参数fd为终端的文件描述符，返回的结果保存在termios结构体中
   */
  tcgetattr(serial_fd, &options);
  /**2. 修改所获得的参数*/
  options.c_cflag |= (CLOCAL | CREAD);//设置控制模式状态，本地连接，接收使能
  options.c_cflag &= ~CSIZE;//字符长度，设置数据位之前一定要屏掉这个位
  options.c_cflag &= ~CRTSCTS;//无硬件流控
  options.c_cflag |= CS8;//8位数据长度
  options.c_cflag &= ~CSTOPB;//1位停止位
  options.c_iflag |= IGNPAR;//无奇偶检验位
  options.c_oflag = 0; //输出模式
  options.c_lflag = 0; //不激活终端模式
  cfsetospeed(&options, B57600);//设置波特率

  /**3. 设置新属性，TCSANOW：所有改变立即生效*/
  tcflush(serial_fd, TCIFLUSH);//溢出数据可以接收，但不读
  tcsetattr(serial_fd, TCSANOW, &options);
  return 0;
}

/**
 *串口发送数据
 *@fd:串口描述符
 *@data:待发送数据
 *@datalen:数据长度
 */
int uart_send(int fd, char *data, int datalen)
{
  int len = 0;
  len = write(fd, data, datalen);//实际写入的长度
  if(len == datalen) {
    return len;
  } else {
    tcflush(fd, TCOFLUSH);//TCOFLUSH刷新写入的数据但不传送
    return -1;
  }

  return 0;
}

/**
 *串口接收数据
 *要求启动后，在pc端发送ascii文件
 */
int uart_recv(int fd, char *data, int datalen)
{
  int len=0, ret = 0;
  fd_set fs_read;
  struct timeval tv_timeout;

  FD_ZERO(&fs_read);
  FD_SET(fd, &fs_read);
  tv_timeout.tv_sec  = (10*20/baudRete+2);
  tv_timeout.tv_usec = 0;

  ret = select(fd+1, &fs_read, NULL, NULL, &tv_timeout);
  printf("ret = %d\n", ret);
  //如果返回0，代表在描述符状态改变前已超过timeout时间,错误返回-1
  if (ret < 0) {
    perror("---select");
    return -1;
  }else if (ret == 0){
    perror("select timeout");
    return -1;
  }

  if (FD_ISSET(fd, &fs_read)) {
    len = read(fd, data, datalen);
    printf("len = %d\n", len);
    return len;
  } else {
    perror("select");
    return -1;
  }

  return 0;
}

/* int main(int argc, char **argv) */
/* { */
/*     init_serial(); */

/*     /\* char buf[]="hello world";   *\/ */
/*     /\* char buf1[10];   *\/ */
/*     /\* uart_send(serial_fd, buf, 10);   *\/ */
/*     /\* printf("\n");   *\/ */

/*     /\* uart_recv(serial_fd, buf1, 10);   *\/ */

/*     /\* printf("uart receive %s\n", buf1);   *\/ */
/*     /\* close(serial_fd);   *\/ */

/*     char buf1[2] = {0}; */
/* int ret = 0; */
/*     while(1){ */
/*       bzero(buf1,2); */
/*       uart_send(serial_fd, "0", 1); */
/*       ret = uart_recv(serial_fd, buf1, 1000); */
/*       printf("buf1 = %s  ;   len = %d\n", buf1, ret); */

/*       sleep(1); */
/*       bzero(buf1,2); */
/*       uart_send(serial_fd, "1", 1); */
/*       ret = uart_recv(serial_fd, buf1, 1000); */
/*       printf("buf1 = %s  ;   len = %d\n", buf1 , ret ); */
/*     } */
/*     return 0; */
/* } */
