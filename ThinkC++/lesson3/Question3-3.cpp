/* 
 *     Created:  14  7月 2013 09:43:09
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int wordNum(string word)
{
    char *wordcstr = const_cast<char*>(word.c_str());
    int l = word.length();

    int num = 0;
    for (int i = 0; i < l; i++)
    {
	char c = wordcstr[i];
	num += c;
    }
    return num;
}


int main(int argc, char *argv[])
{
    ifstream in("Question3-3.cpp");
    string word;

    int num = 0;
    while(in >> word)
    {
	num = wordNum(word);
	cout << num << endl;
    }
    return 0;
}














