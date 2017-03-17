// -*- C++ -*-

/* @(#)TcpClient.h
 */

#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H 1


#ifdef __cplusplus
extern "C"
{
#endif

    extern void *TcpClientRun(void *);

    int readLocalMessage(char **str);
    void filterLocalData(const char *data);
    void closeConnect();


#ifdef __cplusplus
}
#endif


#endif /* _TCPCLIENT_H */

