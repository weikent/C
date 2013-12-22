// -*- C++ -*-

/* @(#)dataparse.h
 */

#ifndef _DATAPARSE_H
#define _DATAPARSE_H 1


#include <string>
#include <deque>
//#include "IPC_message/message.h"
#include "message.h"

using namespace std;

class dataparse
{
public:
    dataparse();
    virtual ~dataparse();

public:
//    struct Sys *createStruct();
    void createStruct();
    void deleteStruct();
    int parse(const char *jsondata); 
    char * checkOptCode(const char *jsonData);
//    int packageSignalCtrl(SrvCmd cmdStruct, string &str);
    int packageMutipleCtrl(deque<SrvCmd> cmdQue, string &str);
    int packageMutipleSensor(deque<MeteringData> dataQue, string &str);
//    int packageSignalSensor(MeteringData meterStruct, string &str);

    deque<SrvCmd> cmdQueue;
private:
    string getCurrentTime();
    string intToStr(int i);
    string& trim(string &str, string oldstr,string::size_type pos = 0);

private:
    SrvCmd theCmd;

};

#endif /* _DATAPARSE_H */

