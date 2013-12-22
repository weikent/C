/* @(#)getOtherID.h
 */

#ifndef _GETOTHERID_H
#define _GETOTHERID_H 1

class getOtherID
{
public:
    getOtherID();
    virtual ~getOtherID();
private:
    int isdir(const char *path);
    char * getfile(const char *path);
    char *readstatus(const char *filename);
public:
    int getID(const char *filename);
};

#endif /* _GETOTHERID_H */

