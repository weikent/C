// -*- C++ -*-

/* @(#)connectServer.h
 */

#ifndef _CONNECTSERVER_H
#define _CONNECTSERVER_H 1

class connectServer
{
public:
    connectServer();
    virtual ~connectServer();
public:
    int socket_id;
    string serverIP;
    int serverPort;
public:
    int shortConnect();
    int longConnect();
private:
    int createSocket();
    int socketConnectServer();
    int socketRead();
    int SocketWrite();

    
};

#endif /* _CONNECTSERVER_H */

