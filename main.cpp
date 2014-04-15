/*
 * =====================================================================================
 *
 *    Filename:  main.cpp
 *
 * Description:  此函数用来测试如何设置系统时间
 *
 *    
 *
 *     Version:  0.1
 *     Created:  Tue Apr 15 16:30:10 2014
 *
 *     Authors:  世建 魏 ,  
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   世建 魏  Tue Apr 15 16:30:10 2014 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

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

