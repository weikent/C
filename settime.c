#include <stdio.h>
#include <time.h>
#include <string.h>
int main(char argc,char **argv)
{
	time_t tm;
	struct tm tm_time;
	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	tm_time.tm_sec = 44;
	tm_time.tm_min = 5;
	tm_time.tm_hour = 10;
	tm_time.tm_mon=8-1;
	tm_time.tm_year=2013-1900;
	tm_time.tm_mday=24;
	tm = mktime(&tm_time);
	stime(&tm);
	return 0;
}
