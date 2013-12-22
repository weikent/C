/* 
 *     Created:  25  7月 2013 10:09:42
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include "phoneDataParse.h"
#include <stdlib.h>
#include <string>
#include "string.h"
#include <iostream>
#include "cJSON.h"
#include <cstdio>
#include <deque>
#include <sys/time.h>    
#include <sstream>

phoneDataParse::phoneDataParse()
{
}

phoneDataParse::~phoneDataParse()
{
}



char *phoneDataParse::checkOptCode(const char *jsonData)
{
    cJSON *json;
    json = cJSON_Parse(jsonData);
    if (json)
    {
	int n_sysKey = cJSON_GetArraySize(json);
	int i_sysKey = 0;

	cJSON *jsonSys;
	
	char *key;
	char *value;
	for (i_sysKey = 0; i_sysKey < n_sysKey; i_sysKey++)
	{
	    jsonSys = cJSON_GetArrayItem(json, i_sysKey);

	    if (NULL == jsonSys)
	    {
		
	    }
	
	    if (cJSON_String == jsonSys->type)
	    {
		key = jsonSys->string;
		value = jsonSys->valuestring;

		if (strcmp(key, "optCode") == 0)
		{
		    return value;
		}
	    }
	}
    }
}


int phoneDataParse::parsePairData(const char *jsonData, PairInfo &pairInfo)
{
    cJSON *json;
    json = cJSON_Parse(jsonData);
    if (json)
    {
	int n_sysKey = cJSON_GetArraySize(json);
	int i_sysKey = 0;

	cJSON *jsonSys;
	char *key;
	char *value;
	for (i_sysKey = 0; i_sysKey < n_sysKey; i_sysKey++)
	{
	    jsonSys = cJSON_GetArrayItem(json, i_sysKey);

	    if (NULL == jsonSys)
	    {
		
	    }
	    else
	    {
		if (cJSON_String == jsonSys->type)
		{
		    key = jsonSys->string;
		    value = jsonSys->valuestring;
		    if (strcmp(key, "sysID") == 0)
		    {
			strcpy(pairInfo.sys_id, value);
		    }

		    if (strcmp(key, "optCode") == 0)
		    {
			strcpy(pairInfo.op_code, value);
		    }

		    if (strcmp(key, "tiemOut") == 0)
		    {
			pairInfo.timeOut = atol(value);
		    }
		    
		    if (strcmp(key, "SSID") == 0)
		    {
			strcpy(pairInfo.ssid, value);
		    }
		    
		    if (strcmp(key, "password") == 0)
		    {
			strcpy(pairInfo.password, value);
		    }

		    if (strcmp(key, "DHCPStatus") == 0)
		    {
			pairInfo.dhcp_status = atoi(value);
		    }
		    
		    if (strcmp(key, "IPAdress") == 0)
		    {
			strcpy(pairInfo.ip_addr, value);
		    }
		    
		    if (strcmp(key, "mask") == 0)
		    {
			strcpy(pairInfo.mask, value);
		    }
		    
		    if (strcmp(key, "geteway") == 0)
		    {
			strcpy(pairInfo.gete_way, value);
		    }

		    if (strcmp(key, "DNS1") == 0)
		    {
			strcpy(pairInfo.dns1, value);
		    }

		    if (strcmp(key, "DNS2") == 0)
		    {
			strcpy(pairInfo.dns2, value);
		    }

		    if (strcmp(key, "serverIP") == 0)
		    {
 			strcpy(pairInfo.server_ip, value);
		    }
		    
		    if (strcmp(key, "serverPort") == 0)
		    {
			pairInfo.server_port = atoi(value);
		    }
		}
	    }
	}
    }
    else
    {
	printf ("error parse [%s]\n", cJSON_GetErrorPtr());
	return -1;
    }
    return 0;
}


int phoneDataParse::packageWiFiInfo(deque<WiFiInfo> wifilist, string &str)
{

    cJSON *wifiList;
    cJSON *wifiInfoJson;
    cJSON *wifiInfoArray;
    
    wifiInfoArray = cJSON_CreateArray();

    char *temp;
    while(wifilist.size())
    {
	wifiInfoJson = cJSON_CreateObject();

	cJSON_AddItemToObject(wifiInfoJson, "SSId", cJSON_CreateString(wifilist.front().ssid));
	temp = const_cast<char *>(intToStr(wifilist.front().rssi).c_str()); 
	cJSON_AddItemToObject(wifiInfoJson, "RSSI", cJSON_CreateString(temp));
	cJSON_AddItemToObject(wifiInfoJson, "encryption", cJSON_CreateString(wifilist.front().encryption));
	cJSON_AddItemToArray(wifiInfoArray, wifiInfoJson);
	wifilist.pop_front();
    }

    wifiList = cJSON_CreateObject();
    cJSON_AddItemToObject(wifiList, "sysID", cJSON_CreateString("0150"));
    cJSON_AddItemToObject(wifiList, "optCode", cJSON_CreateString("0020"));
    cJSON_AddItemToObject(wifiList, "timeOut", cJSON_CreateString("22222222222"));
    cJSON_AddItemToObject(wifiList, "WiFiListArray", wifiInfoArray);

    str = cJSON_Print(wifiList);

    return 0;
    
}

string phoneDataParse::intToStr(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
