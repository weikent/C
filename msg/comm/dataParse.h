// -- C++ --

/* @(#)dataParse.h
 */

#ifndef _DATAPARSE_H
#define _DATAPARSE_H 1

class dataParse
{
public:
    dataParse();
    virtual ~dataParse();
public:
    int serverDataParse(const char *serverData);
};
//    int serverDataParse(const char *serverData);
//int serverDataRePackage(const char *serverData, struct localStruct *localStruct);


#endif /* _DATAPARSE_H */

