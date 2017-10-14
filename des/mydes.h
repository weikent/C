typedef unsigned long long LL;
typedef unsigned char byte;
typedef enum 
{ 
	DES_ENCRYPT = 0, 
	DES_DECRYPT = 1 
}DES_MODE; 

void desEn(LL& data, LL& en_data);
void desDe(LL& en_data, LL& de_data);
LL key2LL(byte*a);
void LL2Key(LL a,byte*ret);