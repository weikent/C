/* 
 *     Created:  06  8月 2013 16:48:05
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int ret = system("ping 192.168.8.253 -c4");
    if (WEXITSTATUS( ret ) == 0)
    {
	printf ("OK\n");
    }
    else
    {
	printf("Error\n");
    }
    
    return 0;
}


