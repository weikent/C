/* 
 *     Created:  14  7月 2013 10:01:36
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>

#include <string>
#include <iostream>

using namespace std;


void fun1(string *str)
{
    *str = "new string in fun1";
}


void fun2(string &str)
{
    str = "new string in fun2";
}


int main(int argc, char *argv[])
{
    string str;
    str = "no change";
    fun1(&str);
    cout << str << endl;
    fun2(str);
    cout << str << endl;
    return 0;
}


