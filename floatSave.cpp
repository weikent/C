/* 
 *     Created:  13  7月 2013 11:41:45
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include<iostream>
using namespace std;
int main()
{
    float f = 0.1F;
    unsigned int *pa = (unsigned int*)(&f);
    for(int i = 31; i >= 0; i--)
    {
	cout << ((*pa >> i) & 1) << (i == 31 || i == 23 ? "-" : " ");
    }
    cout << "\n";
    return 0;
}
