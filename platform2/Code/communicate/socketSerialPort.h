// -*- C++ -*-

/* @(#)SocketSerialPort.h
 */

#ifndef _SOCKETSERIALPORT_H
#define _SOCKETSERIALPORT_H 1



extern int serial_fd;
#define SERIAL_PORT "/dev/ttyS2"

#ifdef __cplusplus
extern "C"
{
#endif

  int init_serial(void);
  int uart_send(int fd, char *data, int datalen);
  int uart_recv(int fd, char *data, int datalen);
  
#ifdef __cplusplus
}
#endif


#endif /* _SOCKETSERIALPORT_H */

