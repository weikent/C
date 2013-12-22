/* 
 *     Created:  17  8月 2013 17:03:41
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
        char text[] = "{\"optCode\":\"heart\",\"devID\":\"34bca6010203\"}";
//    char text[] = "{\"sysID\":\"0010\",\"optCode\":\"0010\",\"timeOut\":\"111111111\",\"getWiFiList\":\"1\"}";
//    char text[] = "{\"sysID\":\"0010\",\"optCode\":\"0020\",\"timeOut\":\"111111111\",\"SSID\":\"sisco123\",\"password\":\"startech1\",\"DHCPStatus\":\"0\",\"IPAdress\":\"192.168.1.111\",\"mask\":\"255.255.255.0\",\"gateway\":\"192.168.1.1\",\"DNS1\":\"192.168.1.1\",\"DNS2\":\"192.168.1.1\",\"serverIP\":\"10.10.10.112\",\"serverPort\":\"14567\"}";
	printf ("text len = %d\n",strlen(text));
    return 0;
}

