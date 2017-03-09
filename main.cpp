#include "mydes.h"
#include <iostream>
using namespace std;

void main()
{
	byte data[]={29,253,123,81,93,119,91,6};
	byte en[8];
	byte de[8];
	LL d=key2LL(data);
	LL e;
	LL x;
	for (int i = 0; i < 8; i++)cout<<(int)data[i]<<" ";
	cout<<endl;
	desEn(d,e);
	LL2Key(e,en);
	for (int i = 0; i < 8; i++)cout<<(int)en[i]<<" ";
	cout<<endl;
	desDe(e,x);
	LL2Key(x,de);
	for (int i = 0; i < 8; i++)cout<<(int)de[i]<<" ";
	cout<<endl;
	system("pause");
}