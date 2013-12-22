// -*- C++ -*-

/* @(#)phoneDataParse.h
 */

#ifndef _PHONEDATAPARSE_H
#define _PHONEDATAPARSE_H 1

#include "global.h"
#include <deque>
#include <string>

using namespace std;

class phoneDataParse
{
public:
    phoneDataParse();
    virtual ~phoneDataParse();
public:
    char * checkOptCode(const char *jsonData);
    int parsePairData(const char *jsonData, PairInfo &pairInfo);
    int packagePairResult(string &str, string errCode);
    int packageWiFiInfo(deque<WiFiInfo> wifilist, string &str);
private:
    string intToStr(int i);
};

#endif /* _PHONEDATAPARSE_H */

