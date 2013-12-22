/* 
 *     Created:  14  7月 2013 11:21:21
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>

#include <iostream>

using namespace std;

union Primitives
{
    char c;
    short s;
    int i;
    long l;
    float f;
    double d;
    long double ld;
};


int main(int argc, char *argv[])
{
    Primitives p[10];
    Primitives *pp = p;
    
    cout << "sizeof(Primitives) = " << sizeof(Primitives) << endl;

    cout << "pp = " << pp << endl;

    pp ++;
    cout << "pp = " << (long)pp << endl;
    return 0;
}


















