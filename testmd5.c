/* 
 *     Created:  16 10月 2013 15:04:56
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdio.h>
#include <string.h>

#include "md5.h"

int main ()
{
 char tmp[128];
 unsigned char digest[16];
 
 MD5_CTX context;
 
 scanf("%s",tmp);
  
 MD5Init (&context);
 MD5Update (&context, (unsigned char*)tmp, strlen(tmp));
 MD5Final (digest,&context);

 printf("MD5Value:");
 for(int i=0; i<16; ++i)
 {
  printf("%02X",digest[i]);
 }
 printf("\n");

 return 0;
}
