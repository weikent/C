// -*- C++ -*-

/* @(#)APModel.h
 */

#ifndef _APMODEL_H
#define _APMODEL_H 1

#include "Thread.h"
#include <string>

using namespace std;


class APModel : public Thread
{
public:
    APModel();
    virtual ~APModel();
    virtual void *run();

private:
    void startServer();
    void filterData(const char *buffer);
    int getWIFIList(string &str);
    int setupDevice(string &str);
};

#endif /* _APMODEL_H */

