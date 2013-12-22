// -- C++ --

/* @(#)dataparse.h
 */

#ifndef _DATAPARSE_H
#define _DATAPARSE_H 1


#include <string>
#include <deque>


using namespace std;

/* struct Act */
/* { */
/*     string actType; */
/*     string actValue; */
/* }; */


/* struct Cont */
/* { */
/*     string contID; */
/*     string contType; */
/*     string numOfAct; */
/*     struct Act *actions; */
/* }; */

/* struct Dev */
/* { */
/*     string devID; */
/*     string numofCont; */
/*     struct Cont *controls; */
/* }; */

/* struct Sys */
/* { */
/*     string sysID; */
/*     string seqOfIns; */
/*     string numOfDev; */
/*     string optCode; */
/*     string timeout; */
/*     struct Dev *devices; */
/* }; */

struct SrvCmd {
    // Layer 1
    char sysID[8];
    size_t insSeqNo;
    size_t devNo;
    char opCode[8];
    // Layer 2
    char devID[16];
    size_t ctrlNo;
    // Layer 3
    char ctrlID[8];
    char ctrlTyp[8];
    size_t actNo;
    // Layer 4
    char actType[8];
    char actVal[8];
    char actUnit[8];
    size_t actTime;
    float actPrecs;
    float actMin;
    float actMax;
    char actStat[8];
    time_t actStatTime;
};

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
//    struct Sys *sys;
    deque<SrvCmd> cmdQueue;
private:
    SrvCmd theCmd;

};

#endif /* _DATAPARSE_H */

