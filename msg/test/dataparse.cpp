/* 
 *     Created:  08 Jul 2013 12:47:37
 *     Author:   root@UbuntuMac
 Description:  
*/

//#include <config.h>
#include "dataparse.h"
#include <stdlib.h>
#include <string>
#include "string.h"
#include <iostream>
#include "cJSON.h"
#include <cstdio>
#include <deque>



dataparse::dataparse()
{
}

dataparse::~dataparse()
{
}

/* int parse(struct B &b) */
/* { */

/* b.c = "c"; */
/* b.pa = new struct A; */
/* b.pa->a = "a"; */
/* b.pa->b = "b"; */
/* } */


/* char * checkOptCode(const char *jsonData) */
/* { */
/*     cJSON *json; */
/*     json = cJSON_Parse(jsonData); */
/*     if (json) */
/*     { */
/* 	int n_sysKey = cJSON_GetArraySize(json); */
/* 	int i_sysKey = 0; */

/* 	cJSON *jsonSys; */
	
/* 	char *key; */
/* 	char *value; */
/* 	for (i_sysKey = 0; i_sysKey < n_sysKey; i_sysKey++) */
/* 	{ */
/* 	    jsonSys = cJSON_GetArrayItem(json, i_sysKey); */

/* 	    if (NULL == jsonSys) */
/* 	    { */
		
/* 	    } */
	
/* 	    if (cJSON_String == jsonSys->type) */
/* 	    { */
/* 		key = jsonSys->string; */
/* 		value = jsonSys->valuestring; */

/* 		if (strcmp(key, "optCode") == 0) */
/* 		{ */
/* 		    return value; */
/* 		} */
/* 	    } */
/* 	} */
/*     } */
/* } */


int dataparse::parse(const char *jsonData)
{
    cJSON *json;
	
    json=cJSON_Parse(jsonData);

    if (json)
    {
/* printf ("%d\n", json->type); */
/* printf ("%d\n", json->valueint); */
/* printf ("%d\n", cJSON_GetArraySize(json)); */

	int n_sysKey = cJSON_GetArraySize(json); 
	int i_sysKey = 0; 

//对应json的4层结构
	cJSON *jsonSys;
	cJSON *jsonDev;
	cJSON *jsonContOrSen;
	cJSON *jsonActOrPara;

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

		if (strcmp(key, "sysID") == 0)
		{
		    /* theCmd.sysID = value; */
		    strcpy(theCmd.sysID, value);
		}
		if (strcmp(key, "seqOfIns") == 0)
		{
		    /* sys.seqOfIns = atoi(value); */
		    theCmd.insSeqNo = atoi(value);
		}
		if (strcmp(key, "timeout") == 0)
		{
		    
		}
		if (strcmp(key, "optCode") == 0)
		{
		    /* sys.opCode = value; */
		    strcpy(theCmd.opCode, value);
		}
		if (strcmp(key, "numOfDev") ==0)
		{
		    /* sys.devNo = atoi(value); */
		    theCmd.devNo = atoi(value);
		}
	    }
	}

	n_sysKey = cJSON_GetArraySize(json); 
	i_sysKey = 0; 

	for (i_sysKey = 0; i_sysKey < n_sysKey; i_sysKey++)
	{
	    jsonSys = cJSON_GetArrayItem(json, i_sysKey);

	    if (cJSON_Array == jsonSys->type)
	    {
		int n_dev = cJSON_GetArraySize(jsonSys);
		int i_dev = 0;


		for (i_dev = 0; i_dev < n_dev ; i_dev++)
		{
		    jsonDev = cJSON_GetArrayItem(jsonSys, i_dev);
		    
		    if (cJSON_Object == jsonDev->type)
		    {
			int n_devKey = cJSON_GetArraySize(jsonDev);
			int i_devKey = 0;
			cJSON *jsonDevKey;
			for (i_devKey = 0; i_devKey < n_devKey; i_devKey++)
			{
			    jsonDevKey = cJSON_GetArrayItem(jsonDev, i_devKey);
			    
			    if (cJSON_String == jsonDevKey->type)
			    {
				key = jsonDevKey->string;
				value = jsonDevKey->valuestring;

				if (strcmp(key, "devID") == 0)
				{
				    /* device.devID = value; */
				    strcpy(theCmd.devID, value);
				}

				if (strcmp(key, "numOfCont") == 0)
				{
				    /* device.ctrlNo = atoi(value); */
				    theCmd.ctrlNo = atoi(value);
				}
			    }
			}

			n_devKey = cJSON_GetArraySize(jsonDev);
			i_devKey = 0;

			for (i_devKey = 0; i_devKey < n_devKey; i_devKey++)
			{
			    jsonDevKey = cJSON_GetArrayItem(jsonDev, i_devKey);

			    if (cJSON_Array == jsonDevKey->type)
			    {
				int n_contOrSen = cJSON_GetArraySize(jsonDevKey);
				int i_contOrSen = 0;


				for (i_contOrSen = 0; i_contOrSen < n_contOrSen; i_contOrSen++)
				{
				    jsonContOrSen = cJSON_GetArrayItem(jsonDevKey, i_contOrSen);
				    if (cJSON_Object == jsonContOrSen->type)
				    {
					int n_contOrSenKey = cJSON_GetArraySize(jsonContOrSen);
					int i_contOrSenKey = 0;
					cJSON *jsonContOrSenKey;
					for (i_contOrSenKey = 0; i_contOrSenKey < n_contOrSenKey; i_contOrSenKey++)
					{
					    jsonContOrSenKey = cJSON_GetArrayItem(jsonContOrSen, i_contOrSenKey);
					    
					    if (cJSON_String == jsonContOrSenKey->type)
					    {
						key = jsonContOrSenKey->string;
						value = jsonContOrSenKey->valuestring;
						if (strcmp(key, "contID") == 0)
						{
						    /* control.ctrlID = value; */
						    strcpy(theCmd.ctrlID, value);
						}
						
						if (strcmp(key, "contType") == 0)
						{
						    /* control.ctrlTyp = value; */
						    strcpy(theCmd.ctrlTyp, value);
						}

						if (strcmp(key, "numOfAct") == 0)
						{
						    /* control.actNo = atoi(value); */
						    theCmd.actNo = atoi(value);
						}
					    }
					}

					n_contOrSenKey = cJSON_GetArraySize(jsonContOrSen);
					i_contOrSenKey = 0;

					for (i_contOrSenKey = 0; i_contOrSenKey < n_contOrSenKey; i_contOrSenKey++)
					{
					    jsonContOrSenKey = cJSON_GetArrayItem(jsonContOrSen, i_contOrSenKey);
					  
					    if (cJSON_Array == jsonContOrSenKey->type)
					    {
						int n_actOrPara = cJSON_GetArraySize(jsonContOrSenKey);
						int i_actOrPara = 0;

						for (i_actOrPara = 0; i_actOrPara < n_actOrPara; i_actOrPara++)
						{
						    jsonActOrPara = cJSON_GetArrayItem(jsonContOrSenKey, i_actOrPara);
						    if (cJSON_Object == jsonActOrPara->type)
						    {
							int n_actOrParaKey = cJSON_GetArraySize(jsonActOrPara);
							int i_actOrParaKey = 0;
							cJSON *jsonActOrParaKey;
							for (i_actOrParaKey = 0; i_actOrParaKey < n_actOrParaKey; i_actOrParaKey++)
							{
							    jsonActOrParaKey = cJSON_GetArrayItem(jsonActOrPara, i_actOrParaKey);
					    
							    if (cJSON_String == jsonActOrParaKey->type)
							    {
								key = jsonActOrParaKey->string;
								value = jsonActOrParaKey->valuestring;
								if (strcmp(key, "actType") == 0)
								{
								    /* action.actType = value; */
								    strcpy(theCmd.actType, value);
								}
						
								if (strcmp(key, "actValue") == 0)
								{
								    /* action.actVal = value; */
								    strcpy(theCmd.actVal, value);
								}
							    }
							}
						    }
						    cmdQueue.push_back(theCmd);
						}
					        /* control.theAct.push_back(action); */
					    }
					}
				    }
				}
				/* device.theCtrl.push_back(control); */
			    }
			}
		    }
		}
		/* sys.theDev.push_back(device); */
	    }
	}
    }
    else
    {
	printf ("error parse [%s]\n", cJSON_GetErrorPtr());
	return -1;
    }

/* sys.sysID = "sysID"; */

/* sys.devices = new struct Dev[2]; */
/* sys.devices->devID = "1"; */
/* sys.devices->controls = new struct Cont[2]; */
/* sys.devices->controls->contID = "Device1 control1"; */
/* sys.devices->controls->contType = "Device1 controlType1"; */
/* sys.devices->controls->numOfAct = "2"; */
/* (sys.devices->controls + 1)->contID = "Device1 control2"; */
/* (sys.devices->controls + 1)->contType = "Device1 controleType2"; */
/* (sys.devices->controls + 1)->numOfAct = "2"; */

/* (sys.devices + 1)->devID = "2"; */
/* (sys.devices + 1)->controls = new struct Cont[2]; */
/* (sys.devices + 1)->controls->contID = "Device2 control1"; */
/* (sys.devices + 1)->controls->contType = "Device2 controlType1"; */
/* (sys.devices + 1)->controls->numOfAct = "2"; */
/* ((sys.devices + 1)->controls + 1)->contID = "Device2 control2"; */
/* ((sys.devices + 1)->controls + 1)->contType = "Device2 controlType2"; */
/* ((sys.devices + 1)->controls + 1)->numOfAct = "2"; */

    return 0;
}

/* int package(struct Sys &sys, char *jsonData) */
/* { */

    
/*     return 0; */
/* } */

/* struct Sys * dataparse::createStruct() */
/* { */
/*     return new struct Sys; */
/* } */


void dataparse::createStruct()
{
//    sys = new struct Sys;
}

void dataparse::deleteStruct()
{
    
    /* cout<<"delete"<<endl; */


    /* int deviceNum = atoi(sys->numOfDev.c_str()); */
    
    /* for (int i = 0; i < deviceNum; i++) */
    /* { */
    /* 	int controlNum = atoi((sys->devices + i)->numofCont.c_str()); */
    /* 	for (int j = 0; j < controlNum; j ++) */
    /* 	{ */
    /* 	    delete []((sys->devices + i)->controls + j)->actions; */
    /* 	    ((sys->devices + i)->controls + j)->actions = 0; */
    /* 	} */
    /* 	delete [](sys->devices + i)->controls; */
    /* 	(sys->devices + i)->controls = 0; */
    /* } */
    
    /* delete [](sys->devices); */
    /* delete sys; */
}
