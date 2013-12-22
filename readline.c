/* 
 *     Created:  11 Jun 2013 14:43:41
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
#include "readline.h"

#include <ctype.h>
#include <stdio.h>
#include "readline.h"


int read_line(char str[], int n)
{
    int ch, i = 0;
    
    while (isspace(ch = getchar()));
    while (ch != '\n' && ch != EOF)
    {
	if (i < n)
	{
	    str[i++] = ch;
	}
	ch = getchar();
    }
    str[i] = '\0';
    return i;
}

