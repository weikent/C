// -*- C++ -*-

/* @(#)utility.h
 */

#ifndef _UTILITY_H
#define _UTILITY_H 1

#include <string>
using namespace std;

class utility
{
public:
    utility();
    virtual ~utility();
public:
    int getMAC(char *name, string &mac);
    string& trim(string &str, string oldstr,string::size_type pos = 0);


};

#endif /* _UTILITY_H */

