/* 
 *     Created:  07  8月 2013 16:21:02
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;


int SetSystemTime(char *dt)
{
    string str = dt;
    string strDate = "";
    string strTime = "";
    int pos = str.find(" ");
    if (pos != str.npos)
    {
	strDate = str.substr(0, pos);
	strTime = str.substr(pos + 1, str.length());
    }
    else
    {
	return -1;
    }

    char setTime[100] = "";
    sprintf(setTime, "date -s \"%s %s\"", strDate.c_str(), strTime.c_str());
    system(setTime);
    return 0;
}



int main(int argc, char *argv[])
{
    char *dt = "2013-8-7 16:24:44";
    SetSystemTime(dt);
    return 0;
}

