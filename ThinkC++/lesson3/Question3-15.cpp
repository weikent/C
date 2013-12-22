/* 
 *     Created:  14  7月 2013 10:23:11
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>

#include <string>
#include <iostream>


using namespace std;

typedef struct structObj
{
    string str1;
    string str2;
    int i;
} structObj;

int main(int argc, char *argv[])
{
    structObj struct1;
    structObj *struct2 = &struct1;

  
    struct1.str1 = "str1";
    struct1.str2 = "str2";
    struct1.i = 10;
    cout << "struct1.str1 = " << struct1.str1 
	 << "struct1.str2 = " << struct1.str2
	 << "struct1.i = " << struct1.i << endl;

  
    struct2->str1 = "new str1";
    struct2->str2 = "new str2";
    struct2->i = 20;
    cout << "struct2.str1 = " << struct2->str1 
	 << "struct2.str2 = " << struct2->str2
	 << "struct2.i = " << struct2->i << endl;
    
    return 0;
}



