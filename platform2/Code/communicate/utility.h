// -*- C++ -*-

/* @(#)utility.h
 */

#ifndef _UTILITY_H
#define _UTILITY_H 1

#include <vector>
#include <string>

using namespace std;

class Utility
{
public:
    Utility();
    virtual ~Utility();
public:
    int GetMAC(char *name, string &mac);
    string& Trim(string &str, string oldstr,string::size_type pos = 0);
    string& replace_all(string& str, const string& old_value, const string& new_value);
    int GetCurrentTime(char *time);
    string GetIP(char *name);	
    int SetSystemTime(char *dt);
    int Ping(char *IP);
    int CompareWithWavyLine(char *buffer);
    string IntToStr(int i);
    int StrToInt(string ss);
    int GetRandomSeqOfIns();
    int AddSeqNumInJSON(string &strJson, int seqOfIns);
//    int AddWaveLineAndNumInJSON(string &strJson);
    int AddWaveLineAndNumInJSON(char *strJson);
    int SplitString(const string & strSrc, const string& strDelims, vector<string>& strDest);
    int ReplaceDevID(string &strJson);
    string GetGMTTime(char *timeStr);
    char *gcvt (double number, int ndigit, char *buf);
    int Sleep (int n, char *str);
};

#endif /* _UTILITY_H */

