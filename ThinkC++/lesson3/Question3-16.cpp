/* 
 *     Created:  14  7月 2013 10:46:01
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>

#include <string>
#include <iostream>

using namespace std;


enum ShapType
    {
	circle,
	square,
	rectangle
    };

int main(int argc, char *argv[])
{
    ShapType shape = circle;
    switch (shape)
    {
	case circle:
	    cout << circle << endl;
	case square:
	    cout << square << endl;
	case rectangle:
	    cout << rectangle << endl;

    }
    return 0;
}


