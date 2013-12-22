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
#include <sys/time.h>    
#include <sstream>



dataparse::dataparse()
{
}

dataparse::~dataparse()
{
}


string dataparse::getCurrentTime()
{  
    struct timeval tv;  
    gettimeofday(&tv,NULL);  
    long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;  
    stringstream ss;
    ss << time;
    return ss.str();
}  

string dataparse::intToStr(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}


string& dataparse::trim(string &str, string oldstr,string::size_type pos)
{
    string delim = oldstr; //删除空格或者tab字符
    pos = str.find_first_of(delim, pos);
    if (pos == string::npos)
        return str;
    return trim(str.erase(pos, 1), delim);
}

char * dataparse::checkOptCode(const char *jsonData)
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


int dataparse::parse(const char *jsonData)
{
    cJSON *json;

    json=cJSON_Parse(jsonData);

//    cout << "test6 Json format : " << cJSON_Print(json) << endl;
    if (json)
    {
	int n_sysKey = cJSON_GetArraySize(json); 
	int i_sysKey = 0; 

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
		    strcpy(theCmd.sys_id, value);
		}
		if (strcmp(key, "seqOfIns") == 0)
		{
		    theCmd.ins_seq_no = atoi(value);
		}
		if (strcmp(key, "timeout") == 0)
		{
		    
		}
		if (strcmp(key, "optCode") == 0)
		{
		    strcpy(theCmd.op_code, value);
		}
		if (strcmp(key, "numOfDev") ==0)
		{
		    theCmd.dev_no = atoi(value);
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
				    strcpy(theCmd.dev_id, value);
				}

				if (strcmp(key, "numOfCont") == 0)
				{
				    theCmd.ctrl_no = atoi(value);
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
						    strcpy(theCmd.ctrl_id, value);
						}
						
						if (strcmp(key, "contType") == 0)
						{
						    strcpy(theCmd.ctrl_typ, value);
						}

						if (strcmp(key, "numOfAct") == 0)
						{
						    theCmd.act_no = atoi(value);
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
								    strcpy(theCmd.act_typ, value);
								}
						
								if (strcmp(key, "actValue") == 0)
								{
								    strcpy(theCmd.act_val, value);
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


    return 0;
}


int dataparse::packageMutipleSensor(deque<MeteringData> dataQue, string &str)
{

    cout << "mutiple start" << endl;

    cJSON *sys, *dev, *sensor, *param;
    cJSON *devArray, *sensorArray, *paramArray;
    
    //备份dataQue
    deque<MeteringData> temp1;
    deque<MeteringData> temp2;

    
    //记录有几个不同的dev和sensor
    int devNum = 0;
    int sensorNum = 0;

    int i = dataQue.size();



    sys = cJSON_CreateObject();
    dev = cJSON_CreateObject();
    sensor = cJSON_CreateObject();
    param = cJSON_CreateObject();

    devArray = cJSON_CreateArray();
    sensorArray = cJSON_CreateArray();
    paramArray = cJSON_CreateArray();
 


    char *chTemp = 0;//用于把数字转换成字符串，保存此字符串
    cJSON_AddItemToObject(sys, "sysID", cJSON_CreateString(dataQue.front().sys_id));
//    chTemp = const_cast<char *>(intToStr(dataQue.front().ins_seq_no).c_str());
//    cJSON_AddItemToObject(sys, "seqOfIns", cJSON_CreateString(chTemp));
    chTemp = const_cast<char *>(intToStr(dataQue.front().dev_no).c_str());
    cJSON_AddItemToObject(sys, "numOfDev", cJSON_CreateString(chTemp));
    cJSON_AddItemToObject(sys, "optCode", cJSON_CreateString(dataQue.front().op_code));


    while(dataQue.size())
    {
	//队列中的值备份
	temp1.push_back(dataQue.front());
	temp2.push_back(dataQue.front());
	
	//把每一个param保存到paramArray中，并添加此param对应的sensor_id 和 dev_id
	param = cJSON_CreateObject();
	cJSON_AddItemToObject(param, "paramType", cJSON_CreateString(dataQue.front().param_typ));
	cJSON_AddItemToObject(param, "paramValue", cJSON_CreateString(dataQue.front().param_Val));
	cJSON_AddItemToObject(param, "param_sensor_id", cJSON_CreateString(dataQue.front().sensor_id));
	cJSON_AddItemToObject(param, "param_dev_id", cJSON_CreateString(dataQue.front().dev_id));
	cJSON_AddItemToArray(paramArray, param);
	dataQue.pop_front();
	cout << cJSON_Print(param) << endl;
    }


    //在此循环中把act根据对应的sensor_id 保存到sensor中，并把所有sensor保存到sensorArray中。
    while(temp1.size())
    {
	cout << "temp1.size:" << temp1.size() << endl;

	if (cJSON_GetArraySize(sensorArray) > 0)
	{

	    bool hasExist = false;

	    char *key;
	    char *value;

	    char *sensorID;
	    char *sensor_dev_id;
	    int sensorNum = cJSON_GetArraySize(sensorArray);
	    for (int i = 0; i < sensorNum; i++)
	    {
		cJSON *jsonSensor = cJSON_GetArrayItem(sensorArray, i);
		if (cJSON_Object == jsonSensor->type)
		{
		    int sensorKeyNum = cJSON_GetArraySize(jsonSensor);
		    for (int j = 0; j < sensorKeyNum; j++)
		    {
			cJSON *jsonSensorKey = cJSON_GetArrayItem(jsonSensor, j);
			key = jsonSensorKey->string;
			value = jsonSensorKey->valuestring;
			if (strcmp(key, "senID") == 0)
			{
			    sensorID = value;
			}
			if (strcmp(key, "sensor_dev_id") == 0)
			{
			    sensor_dev_id = value;
			}
			
			if ((strcmp(sensorID, temp1.front().sensor_id) == 0 ) && (strcmp(sensor_dev_id, temp1.front().dev_id) == 0))
			{
			    hasExist = true;
			}
		    }
		}
	    }
	    
	    if (!hasExist)
	    {
		sensor = cJSON_CreateObject();

		cJSON_AddItemToObject(sensor, "senID", cJSON_CreateString(temp1.front().sensor_id));
		cJSON_AddItemToObject(sensor, "senType", cJSON_CreateString(temp1.front().sensor_typ));
		chTemp = const_cast<char *>(intToStr(temp1.front().param_no).c_str());
		cJSON_AddItemToObject(sensor, "numOfParam", cJSON_CreateString(chTemp));
		cJSON_AddItemToObject(sensor, "sensor_dev_id", cJSON_CreateString(temp1.front().dev_id));

		cJSON *tempParamArray = cJSON_CreateArray();

		int paramArrSize = cJSON_GetArraySize(paramArray);

		cJSON *jsonParam;
		cJSON *jsonParamKey;

		char *key;
		char *value;

		char *param_sensor_id;
		char *param_dev_id;
		string empty = "";

		for (int i = 0; i < paramArrSize; i++)
		{
		    jsonParam = cJSON_GetArrayItem(paramArray, i);
		    if (cJSON_Object == jsonParam->type)
		    {
			int paramKeyNum = cJSON_GetArraySize(jsonParam);
			int i_paramKeyNum = 0;

			//创建一个临时cJSON对象，保存jsonParam中的值
			cJSON *tempJsonParam = cJSON_CreateObject();

			param_sensor_id =  const_cast<char*>(empty.c_str());

			param_dev_id =  const_cast<char*>(empty.c_str());
		    
			for (i_paramKeyNum; i_paramKeyNum < paramKeyNum; i_paramKeyNum++)
			{
			    jsonParamKey = cJSON_GetArrayItem(jsonParam, i_paramKeyNum);
			    key = jsonParamKey->string;
			    value = jsonParamKey->valuestring;

			    cout << "key = " << key << endl;
			    cout << "value = " << value << endl;

			    cJSON_AddItemToObject(tempJsonParam, key, cJSON_CreateString(value));

			    if (strcmp(key, "param_sensor_id") == 0)
			    {
				param_sensor_id = value;
			    }
		    
			    if (strcmp(key, "param_dev_id") == 0)
			    {
				param_dev_id = value;
			    }

			    if ((strcmp(param_sensor_id, temp1.front().sensor_id) == 0) && (strcmp(param_dev_id, temp1.front().dev_id) == 0))
			    {

//				cJSON_DeleteItemFromObject(jsonParam, "param_sensor_id");
				cJSON_AddItemToArray(tempParamArray, tempJsonParam);

				cout << "tempParamArray :" << cJSON_Print(tempParamArray) << endl;
//				cJSON_DeleteItemFromArray(paramArray, i);
			    }
			}
		    }
		}
		//希望通过此处循环，删除其中的"param_sensor_id"字段
		int tempParamArraySize = cJSON_GetArraySize(tempParamArray);
		cJSON *jsonTempParam;
		for (int i = 0; i < tempParamArraySize; i++)
		{
		    jsonTempParam = cJSON_GetArrayItem(tempParamArray, i);
		    cJSON_DeleteItemFromObject(jsonTempParam, "param_sensor_id");
		    cJSON_DeleteItemFromObject(jsonTempParam, "param_dev_id");
		}

		cJSON_AddItemToObject(sensor, "paramArray", tempParamArray);
		cJSON_AddItemToArray(sensorArray, sensor);

		cout << "sensorArray :" << cJSON_Print(sensorArray) << endl;
	    }
	}
	else
	{
	    sensor = cJSON_CreateObject();

	    cJSON_AddItemToObject(sensor, "senID", cJSON_CreateString(temp1.front().sensor_id));
	    cJSON_AddItemToObject(sensor, "senType", cJSON_CreateString(temp1.front().sensor_typ));
	    chTemp = const_cast<char *>(intToStr(temp1.front().param_no).c_str());
	    cJSON_AddItemToObject(sensor, "numOfParam", cJSON_CreateString(chTemp));
	    cJSON_AddItemToObject(sensor, "sensor_dev_id", cJSON_CreateString(temp1.front().dev_id));

	    cJSON *tempParamArray = cJSON_CreateArray();

	    int paramArrSize = cJSON_GetArraySize(paramArray);

	    cJSON *jsonParam;
	    cJSON *jsonParamKey;

	    char *key;
	    char *value;

	    char *param_sensor_id;
	    char *param_dev_id;
	    string empty = "";

	    for (int i = 0; i < paramArrSize; i++)
	    {
		jsonParam = cJSON_GetArrayItem(paramArray, i);
		if (cJSON_Object == jsonParam->type)
		{
		    int paramKeyNum = cJSON_GetArraySize(jsonParam);
		    int i_paramKeyNum = 0;

		    //创建一个临时cJSON对象，保存jsonParam中的值
		    cJSON *tempJsonParam = cJSON_CreateObject();

		    param_sensor_id = const_cast<char*>(empty.c_str());

		    param_dev_id = const_cast<char*>(empty.c_str());
		    
		    for (i_paramKeyNum; i_paramKeyNum < paramKeyNum; i_paramKeyNum++)
		    {
			jsonParamKey = cJSON_GetArrayItem(jsonParam, i_paramKeyNum);
			key = jsonParamKey->string;
			value = jsonParamKey->valuestring;

			cout << "key = " << key << endl;
			cout << "value = " << value << endl;

			cJSON_AddItemToObject(tempJsonParam, key, cJSON_CreateString(value));

			if (strcmp(key, "param_sensor_id") == 0)
			{
			    param_sensor_id = value;
			}
		    
			if (strcmp(key, "param_dev_id") == 0)
			{
			    param_dev_id = value;
			}

			if ((strcmp(param_sensor_id, temp1.front().sensor_id) == 0) && (strcmp(param_dev_id, temp1.front().dev_id) == 0))
			{

//				cJSON_DeleteItemFromObject(jsonParam, "param_sensor_id");
			    cJSON_AddItemToArray(tempParamArray, tempJsonParam);

//			    cout << "tempParamArray :" << cJSON_Print(tempParamArray) << endl;
//				cJSON_DeleteItemFromArray(paramArray, i);
			}
		    }
		}
	    }
	    //希望通过此处循环，删除其中的"param_sensor_id"字段
	    int tempParamArraySize = cJSON_GetArraySize(tempParamArray);
	    cJSON *jsonTempParam;
	    for (int i = 0; i < tempParamArraySize; i++)
	    {
		jsonTempParam = cJSON_GetArrayItem(tempParamArray, i);
		cJSON_DeleteItemFromObject(jsonTempParam, "param_sensor_id");
		cJSON_DeleteItemFromObject(jsonTempParam, "param_dev_id");
		
	    }

	    cJSON_AddItemToObject(sensor, "paramArray", tempParamArray);
	    cJSON_AddItemToArray(sensorArray, sensor);
	}

	temp1.pop_front();
    }



    while(temp2.size())
    {
	cout << "temp2.size:" << temp2.size() << endl;

	if (cJSON_GetArraySize(devArray) > 0)
	{

	    bool hasExist = false;

	    char *key;
	    char *value;


	    char *dev_id;

	    int devNum = cJSON_GetArraySize(devArray);

	    for (int i = 0; i < devNum; i++)
	    {
		cJSON *jsonDev = cJSON_GetArrayItem(devArray, i);
		if (cJSON_Object == jsonDev->type)
		{
		    int devKeyNum = cJSON_GetArraySize(jsonDev);
		    for (int j = 0; j < devKeyNum; j++)
		    {
			cJSON *jsonDevKey = cJSON_GetArrayItem(jsonDev, j);
			key = jsonDevKey->string;
			value = jsonDevKey->valuestring;

			if (strcmp(key, "devID") == 0)
			{
			    dev_id = value;
			}
			
			if (strcmp(dev_id, temp2.front().dev_id) == 0)
			{
			    hasExist = true;
			}
		    }
		}
	    }
	    
	    if (!hasExist)
	    {
		dev = cJSON_CreateObject();

		cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(temp2.front().dev_id));
		chTemp = const_cast<char *>(intToStr(temp2.front().sensor_no).c_str());
		cJSON_AddItemToObject(dev, "numOfSen", cJSON_CreateString(chTemp));


		cJSON *tempSensorArray = cJSON_CreateArray();

		int sensorArrSize = cJSON_GetArraySize(sensorArray);

		cJSON *jsonSensor;
		cJSON *jsonSensorKey;

		char *key;
		char *value;

		char *sensor_dev_id;

		for (int i = 0; i < sensorArrSize; i++)
		{
		    jsonSensor = cJSON_GetArrayItem(sensorArray, i);
		    if (cJSON_Object == jsonSensor->type)
		    {
			int sensorKeyNum = cJSON_GetArraySize(jsonSensor);
			int i_sensorKeyNum = 0;

			//创建一个临时cJSON对象，保存jsonParam中的值
			cJSON *tempJsonSensor = cJSON_CreateObject();

			bool saveSensorArrayOK = false;
			for (i_sensorKeyNum; i_sensorKeyNum < sensorKeyNum; i_sensorKeyNum++)
			{
			    jsonSensorKey = cJSON_GetArrayItem(jsonSensor, i_sensorKeyNum);

			    if (cJSON_String == jsonSensorKey->type)
			    {
				key = jsonSensorKey->string;
				value = jsonSensorKey->valuestring;

				cout << "key = " << key << endl;
				cout << "value = " << value << endl;

				cJSON_AddItemToObject(tempJsonSensor, key, cJSON_CreateString(value));

				if (strcmp(key, "sensor_dev_id") == 0)
				{
				    sensor_dev_id = value;
				}
		    
				/* if (strcmp(key, "param_dev_id") == 0) */
				/* { */
				/*     param_dev_id = value; */
				/* } */

			   		    
			    }

			    if (cJSON_Array == jsonSensorKey->type)
			    {
				cJSON_AddItemToObject(tempJsonSensor, "paramArray", jsonSensorKey);
				saveSensorArrayOK = true;
			    }

			    if ((strcmp(sensor_dev_id, temp2.front().dev_id) == 0) && saveSensorArrayOK)
			    {
				cJSON_AddItemToArray(tempSensorArray, tempJsonSensor);
				cout << "tempSensorArray :" << cJSON_Print(tempSensorArray) << endl;
			    }
			}
		    }
		}
		//希望通过此处循环，删除其中的"param_dev_id"字段
		int tempSensorArraySize = cJSON_GetArraySize(tempSensorArray);
		cJSON *jsonTempSensor;
		for (int i = 0; i < tempSensorArraySize; i++)
		{
		
		    jsonTempSensor = cJSON_GetArrayItem(tempSensorArray, i);
		    cJSON_DeleteItemFromObject(jsonTempSensor, "sensor_dev_id");
		}

		cJSON_AddItemToObject(dev, "senArray", tempSensorArray);
		cJSON_AddItemToArray(devArray, dev);

		cout << "devArray :" << cJSON_Print(devArray) << endl;
	    }
	}
	else
	{
	    dev = cJSON_CreateObject();

	    cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(temp2.front().dev_id));
	    chTemp = const_cast<char *>(intToStr(temp2.front().sensor_no).c_str());
	    cJSON_AddItemToObject(dev, "numOfSen", cJSON_CreateString(chTemp));


	    cJSON *tempSensorArray = cJSON_CreateArray();

	    int sensorArrSize = cJSON_GetArraySize(sensorArray);

	    cJSON *jsonSensor;
	    cJSON *jsonSensorKey;

	    char *key;
	    char *value;

	    char *sensor_dev_id;

	    for (int i = 0; i < sensorArrSize; i++)
	    {
		jsonSensor = cJSON_GetArrayItem(sensorArray, i);
		if (cJSON_Object == jsonSensor->type)
		{
		    int sensorKeyNum = cJSON_GetArraySize(jsonSensor);
		    int i_sensorKeyNum = 0;

		    //创建一个临时cJSON对象，保存jsonParam中的值
		    cJSON *tempJsonSensor = cJSON_CreateObject();

		    bool saveSensorArrayOK = false;
		    for (i_sensorKeyNum; i_sensorKeyNum < sensorKeyNum; i_sensorKeyNum++)
		    {
			jsonSensorKey = cJSON_GetArrayItem(jsonSensor, i_sensorKeyNum);

			if (cJSON_String == jsonSensorKey->type)
			{
			    key = jsonSensorKey->string;
			    value = jsonSensorKey->valuestring;

			    cout << "key = " << key << endl;
			    cout << "value = " << value << endl;

			    cJSON_AddItemToObject(tempJsonSensor, key, cJSON_CreateString(value));

			    if (strcmp(key, "sensor_dev_id") == 0)
			    {
				sensor_dev_id = value;
			    }
		    
			    /* if (strcmp(key, "param_dev_id") == 0) */
			    /* { */
			    /*     param_dev_id = value; */
			    /* } */

			   		    
			}

			if (cJSON_Array == jsonSensorKey->type)
			{
			    cJSON_AddItemToObject(tempJsonSensor, "paramArray", jsonSensorKey);
			    saveSensorArrayOK = true;
			}

			if ((strcmp(sensor_dev_id, temp2.front().dev_id) == 0) && saveSensorArrayOK)
			{
			    cJSON_AddItemToArray(tempSensorArray, tempJsonSensor);
			    cout << "tempSensorArray :" << cJSON_Print(tempSensorArray) << endl;
			}
		    }
		}
	    }
	    //希望通过此处循环，删除其中的"param_dev_id"字段
	    int tempSensorArraySize = cJSON_GetArraySize(tempSensorArray);
	    cJSON *jsonTempSensor;
	    for (int i = 0; i < tempSensorArraySize; i++)
	    {
		
		jsonTempSensor = cJSON_GetArrayItem(tempSensorArray, i);
		cJSON_DeleteItemFromObject(jsonTempSensor, "sensor_dev_id");
	    }

	    cJSON_AddItemToObject(dev, "senArray", tempSensorArray);
	    cJSON_AddItemToArray(devArray, dev);

	    cout << "devArray :" << cJSON_Print(devArray) << endl;
	}

	temp2.pop_front();
    }

    cJSON_AddItemToObject(sys, "devArray", devArray);

    str = cJSON_Print(sys);

    int pos = -1;

    string oldstr = "\n";
    trim(str,oldstr, 0);
    oldstr = "\t";
    trim(str, oldstr, 0);
    oldstr = " ";
    trim(str, oldstr, 0);
    oldstr = "\r";
    trim(str, oldstr, 0);

    /* while(str.find(" ")) */
    /* { */
    /* 	pos = str.find(" "); */
    /* 	str.replace(pos, 1, ""); */
    /* } */

    
//    str = temp + str;

    
    return 0;
}


int dataparse::packageMutipleCtrl(deque<SrvCmd> cmdQue, string &str)
{

    cout << "mutiple start" << endl;

    cJSON *sys, *dev, *ctrl, *act;
    cJSON *devArray, *ctrlArray, *actArray;
    
    //备份cmdQue
    deque<SrvCmd> temp1;
    deque<SrvCmd> temp2;

    
    //记录有几个不同的dev和ctrl
    int devNum = 0;
    int ctrlNum = 0;

    int i = cmdQue.size();



    sys = cJSON_CreateObject();
    dev = cJSON_CreateObject();
    ctrl = cJSON_CreateObject();
    act = cJSON_CreateObject();

    devArray = cJSON_CreateArray();
    ctrlArray = cJSON_CreateArray();
    actArray = cJSON_CreateArray();
 


    char *chTemp = 0;//用于把数字转换成字符串，保存此字符串
    cJSON_AddItemToObject(sys, "sysID", cJSON_CreateString(cmdQue.front().sys_id));
    chTemp = const_cast<char *>(intToStr(cmdQue.front().ins_seq_no).c_str());
    cJSON_AddItemToObject(sys, "seqOfIns", cJSON_CreateString(chTemp));
    chTemp = const_cast<char *>(intToStr(cmdQue.front().dev_no).c_str());
    cJSON_AddItemToObject(sys, "numOfDev", cJSON_CreateString(chTemp));
    cJSON_AddItemToObject(sys, "optCode", cJSON_CreateString(cmdQue.front().op_code));


    while(cmdQue.size())
    {
	//队列中的值备份
	temp1.push_back(cmdQue.front());
	temp2.push_back(cmdQue.front());
	
	//把每一个act保存到actArray中，并添加此act对应的ctrl_id
	act = cJSON_CreateObject();
	cJSON_AddItemToObject(act, "actType", cJSON_CreateString(cmdQue.front().act_typ));
	cJSON_AddItemToObject(act, "actValue", cJSON_CreateString(cmdQue.front().act_val));
	cJSON_AddItemToObject(act, "act_ctrl_id", cJSON_CreateString(cmdQue.front().ctrl_id));
	cJSON_AddItemToObject(act, "act_dev_id", cJSON_CreateString(cmdQue.front().dev_id));
	cJSON_AddItemToArray(actArray, act);
	cmdQue.pop_front();
    }


    //在此循环中把act根据对应的ctrl_id 保存到ctrl中，并把所有ctrl保存到ctrlArray中。
    while(temp1.size())
    {
	cout << "temp1.size:" << temp1.size() << endl;

	if (cJSON_GetArraySize(ctrlArray) > 0)
	{

	    bool hasExist = false;

	    char *key;
	    char *value;

	    char *ctrlID;
	    char *ctrl_dev_id;
	    int ctrlNum = cJSON_GetArraySize(ctrlArray);
	    for (int i = 0; i < ctrlNum; i++)
	    {
		cJSON *jsonCtrl = cJSON_GetArrayItem(ctrlArray, i);
		if (cJSON_Object == jsonCtrl->type)
		{
		    int ctrlKeyNum = cJSON_GetArraySize(jsonCtrl);
		    for (int j = 0; j < ctrlKeyNum; j++)
		    {
			cJSON *jsonCtrlKey = cJSON_GetArrayItem(jsonCtrl, j);
			key = jsonCtrlKey->string;
			value = jsonCtrlKey->valuestring;
			if (strcmp(key, "contID") == 0)
			{
			    ctrlID = value;
			}
			if (strcmp(key, "ctrl_dev_id") == 0)
			{
			    ctrl_dev_id = value;
			}
			
			if ((strcmp(ctrlID, temp1.front().ctrl_id) == 0 ) && (strcmp(ctrl_dev_id, temp1.front().dev_id) == 0))
			{
			    hasExist = true;
			}
		    }
		}
	    }
	    
	    if (!hasExist)
	    {
		ctrl = cJSON_CreateObject();

		cJSON_AddItemToObject(ctrl, "contID", cJSON_CreateString(temp1.front().ctrl_id));
		cJSON_AddItemToObject(ctrl, "contType", cJSON_CreateString(temp1.front().ctrl_typ));
		chTemp = const_cast<char *>(intToStr(temp1.front().act_no).c_str());
		cJSON_AddItemToObject(ctrl, "numOfAct", cJSON_CreateString(chTemp));
		cJSON_AddItemToObject(ctrl, "ctrl_dev_id", cJSON_CreateString(temp1.front().dev_id));

		cJSON *tempActArray = cJSON_CreateArray();

		int actArrSize = cJSON_GetArraySize(actArray);

		cJSON *jsonAct;
		cJSON *jsonActKey;

		char *key;
		char *value;

		char *act_ctrl_id;
		char *act_dev_id;
		string empty = "";

		for (int i = 0; i < actArrSize; i++)
		{
		    jsonAct = cJSON_GetArrayItem(actArray, i);
		    if (cJSON_Object == jsonAct->type)
		    {
			int actKeyNum = cJSON_GetArraySize(jsonAct);
			int i_actKeyNum = 0;

			//创建一个临时cJSON对象，保存jsonAct中的值
			cJSON *tempJsonAct = cJSON_CreateObject();

			act_ctrl_id =  const_cast<char*>(empty.c_str());

			act_dev_id =  const_cast<char*>(empty.c_str());
		    
			for (i_actKeyNum; i_actKeyNum < actKeyNum; i_actKeyNum++)
			{
			    jsonActKey = cJSON_GetArrayItem(jsonAct, i_actKeyNum);
			    key = jsonActKey->string;
			    value = jsonActKey->valuestring;

			    cout << "key = " << key << endl;
			    cout << "value = " << value << endl;

			    cJSON_AddItemToObject(tempJsonAct, key, cJSON_CreateString(value));

			    if (strcmp(key, "act_ctrl_id") == 0)
			    {
				act_ctrl_id = value;
			    }
		    
			    if (strcmp(key, "act_dev_id") == 0)
			    {
				act_dev_id = value;
			    }

			    if ((strcmp(act_ctrl_id, temp1.front().ctrl_id) == 0) && (strcmp(act_dev_id, temp1.front().dev_id) == 0))
			    {

//				cJSON_DeleteItemFromObject(jsonAct, "act_ctrl_id");
				cJSON_AddItemToArray(tempActArray, tempJsonAct);

				cout << "tempActArray :" << cJSON_Print(tempActArray) << endl;
//				cJSON_DeleteItemFromArray(actArray, i);
			    }
			}
		    }
		}
		//希望通过此处循环，删除其中的"act_ctrl_id"字段
		int tempActArraySize = cJSON_GetArraySize(tempActArray);
		cJSON *jsonTempAct;
		for (int i = 0; i < tempActArraySize; i++)
		{
		    jsonTempAct = cJSON_GetArrayItem(tempActArray, i);
		    cJSON_DeleteItemFromObject(jsonTempAct, "act_ctrl_id");
		    cJSON_DeleteItemFromObject(jsonTempAct, "act_dev_id");
		}

		cJSON_AddItemToObject(ctrl, "actArray", tempActArray);
		cJSON_AddItemToArray(ctrlArray, ctrl);

		cout << "ctrlArray :" << cJSON_Print(ctrlArray) << endl;
	    }
	}
	else
	{
	    ctrl = cJSON_CreateObject();

	    cJSON_AddItemToObject(ctrl, "contID", cJSON_CreateString(temp1.front().ctrl_id));
	    cJSON_AddItemToObject(ctrl, "contType", cJSON_CreateString(temp1.front().ctrl_typ));
	    chTemp = const_cast<char *>(intToStr(temp1.front().act_no).c_str());
	    cJSON_AddItemToObject(ctrl, "numOfAct", cJSON_CreateString(chTemp));
	    cJSON_AddItemToObject(ctrl, "ctrl_dev_id", cJSON_CreateString(temp1.front().dev_id));

	    cJSON *tempActArray = cJSON_CreateArray();

	    int actArrSize = cJSON_GetArraySize(actArray);

	    cJSON *jsonAct;
	    cJSON *jsonActKey;

	    char *key;
	    char *value;

	    char *act_ctrl_id;
	    char *act_dev_id;
	    string empty = "";

	    for (int i = 0; i < actArrSize; i++)
	    {
		jsonAct = cJSON_GetArrayItem(actArray, i);
		if (cJSON_Object == jsonAct->type)
		{
		    int actKeyNum = cJSON_GetArraySize(jsonAct);
		    int i_actKeyNum = 0;

		    //创建一个临时cJSON对象，保存jsonAct中的值
		    cJSON *tempJsonAct = cJSON_CreateObject();

		    act_ctrl_id = const_cast<char*>(empty.c_str());

		    act_dev_id = const_cast<char*>(empty.c_str());
		    
		    for (i_actKeyNum; i_actKeyNum < actKeyNum; i_actKeyNum++)
		    {
			jsonActKey = cJSON_GetArrayItem(jsonAct, i_actKeyNum);
			key = jsonActKey->string;
			value = jsonActKey->valuestring;

			cout << "key = " << key << endl;
			cout << "value = " << value << endl;

			cJSON_AddItemToObject(tempJsonAct, key, cJSON_CreateString(value));

			if (strcmp(key, "act_ctrl_id") == 0)
			{
			    act_ctrl_id = value;
			}
		    
			if (strcmp(key, "act_dev_id") == 0)
			{
			    act_dev_id = value;
			}

			if ((strcmp(act_ctrl_id, temp1.front().ctrl_id) == 0) && (strcmp(act_dev_id, temp1.front().dev_id) == 0))
			{

//				cJSON_DeleteItemFromObject(jsonAct, "act_ctrl_id");
			    cJSON_AddItemToArray(tempActArray, tempJsonAct);

//			    cout << "tempActArray :" << cJSON_Print(tempActArray) << endl;
//				cJSON_DeleteItemFromArray(actArray, i);
			}
		    }
		}
	    }
	    //希望通过此处循环，删除其中的"act_ctrl_id"字段
	    int tempActArraySize = cJSON_GetArraySize(tempActArray);
	    cJSON *jsonTempAct;
	    for (int i = 0; i < tempActArraySize; i++)
	    {
		jsonTempAct = cJSON_GetArrayItem(tempActArray, i);
		cJSON_DeleteItemFromObject(jsonTempAct, "act_ctrl_id");
		cJSON_DeleteItemFromObject(jsonTempAct, "act_dev_id");
		
	    }

	    cJSON_AddItemToObject(ctrl, "actArray", tempActArray);
	    cJSON_AddItemToArray(ctrlArray, ctrl);
	}

	temp1.pop_front();
    }



    while(temp2.size())
    {
	cout << "temp2.size:" << temp2.size() << endl;

	if (cJSON_GetArraySize(devArray) > 0)
	{

	    bool hasExist = false;

	    char *key;
	    char *value;


	    char *dev_id;

	    int devNum = cJSON_GetArraySize(devArray);

	    for (int i = 0; i < devNum; i++)
	    {
		cJSON *jsonDev = cJSON_GetArrayItem(devArray, i);
		if (cJSON_Object == jsonDev->type)
		{
		    int devKeyNum = cJSON_GetArraySize(jsonDev);
		    for (int j = 0; j < devKeyNum; j++)
		    {
			cJSON *jsonDevKey = cJSON_GetArrayItem(jsonDev, j);
			key = jsonDevKey->string;
			value = jsonDevKey->valuestring;

			if (strcmp(key, "devID") == 0)
			{
			    dev_id = value;
			}
			
			if (strcmp(dev_id, temp2.front().dev_id) == 0)
			{
			    hasExist = true;
			}
		    }
		}
	    }
	    
	    if (!hasExist)
	    {
		dev = cJSON_CreateObject();

		cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(temp2.front().dev_id));
		chTemp = const_cast<char *>(intToStr(temp2.front().ctrl_no).c_str());
		cJSON_AddItemToObject(dev, "numOfCont", cJSON_CreateString(chTemp));


		cJSON *tempCtrlArray = cJSON_CreateArray();

		int ctrlArrSize = cJSON_GetArraySize(ctrlArray);

		cJSON *jsonCtrl;
		cJSON *jsonCtrlKey;

		char *key;
		char *value;

		char *ctrl_dev_id;

		for (int i = 0; i < ctrlArrSize; i++)
		{
		    jsonCtrl = cJSON_GetArrayItem(ctrlArray, i);
		    if (cJSON_Object == jsonCtrl->type)
		    {
			int ctrlKeyNum = cJSON_GetArraySize(jsonCtrl);
			int i_ctrlKeyNum = 0;

			//创建一个临时cJSON对象，保存jsonAct中的值
			cJSON *tempJsonCtrl = cJSON_CreateObject();

			bool saveCtrlArrayOK = false;
			for (i_ctrlKeyNum; i_ctrlKeyNum < ctrlKeyNum; i_ctrlKeyNum++)
			{
			    jsonCtrlKey = cJSON_GetArrayItem(jsonCtrl, i_ctrlKeyNum);

			    if (cJSON_String == jsonCtrlKey->type)
			    {
				key = jsonCtrlKey->string;
				value = jsonCtrlKey->valuestring;

				cout << "key = " << key << endl;
				cout << "value = " << value << endl;

				cJSON_AddItemToObject(tempJsonCtrl, key, cJSON_CreateString(value));

				if (strcmp(key, "ctrl_dev_id") == 0)
				{
				    ctrl_dev_id = value;
				}
		    
				/* if (strcmp(key, "act_dev_id") == 0) */
				/* { */
				/*     act_dev_id = value; */
				/* } */

			   		    
			    }

			    if (cJSON_Array == jsonCtrlKey->type)
			    {
				cJSON_AddItemToObject(tempJsonCtrl, "actArray", jsonCtrlKey);
				saveCtrlArrayOK = true;
			    }

			    if ((strcmp(ctrl_dev_id, temp2.front().dev_id) == 0) && saveCtrlArrayOK)
			    {
				cJSON_AddItemToArray(tempCtrlArray, tempJsonCtrl);
				cout << "tempCtrlArray :" << cJSON_Print(tempCtrlArray) << endl;
			    }	
			}
		    }
		}
		//希望通过此处循环，删除其中的"act_dev_id"字段
		int tempCtrlArraySize = cJSON_GetArraySize(tempCtrlArray);
		cJSON *jsonTempCtrl;
		for (int i = 0; i < tempCtrlArraySize; i++)
		{
		
		    jsonTempCtrl = cJSON_GetArrayItem(tempCtrlArray, i);
		    cJSON_DeleteItemFromObject(jsonTempCtrl, "ctrl_dev_id");
		}

		cJSON_AddItemToObject(dev, "contArray", tempCtrlArray);
		cJSON_AddItemToArray(devArray, dev);

		cout << "devArray :" << cJSON_Print(devArray) << endl;
	    }
	}
	else
	{
	    dev = cJSON_CreateObject();

	    cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(temp2.front().dev_id));
	    chTemp = const_cast<char *>(intToStr(temp2.front().ctrl_no).c_str());
	    cJSON_AddItemToObject(dev, "numOfCont", cJSON_CreateString(chTemp));


	    cJSON *tempCtrlArray = cJSON_CreateArray();

	    int ctrlArrSize = cJSON_GetArraySize(ctrlArray);

	    cJSON *jsonCtrl;
	    cJSON *jsonCtrlKey;

	    char *key;
	    char *value;

	    char *ctrl_dev_id;

	    for (int i = 0; i < ctrlArrSize; i++)
	    {
		jsonCtrl = cJSON_GetArrayItem(ctrlArray, i);
		if (cJSON_Object == jsonCtrl->type)
		{
		    int ctrlKeyNum = cJSON_GetArraySize(jsonCtrl);
		    int i_ctrlKeyNum = 0;

		    //创建一个临时cJSON对象，保存jsonAct中的值
		    cJSON *tempJsonCtrl = cJSON_CreateObject();

		    bool saveCtrlArrayOK = false;
		    for (i_ctrlKeyNum; i_ctrlKeyNum < ctrlKeyNum; i_ctrlKeyNum++)
		    {
			jsonCtrlKey = cJSON_GetArrayItem(jsonCtrl, i_ctrlKeyNum);

			if (cJSON_String == jsonCtrlKey->type)
			{
			    key = jsonCtrlKey->string;
			    value = jsonCtrlKey->valuestring;

			    cout << "key = " << key << endl;
			    cout << "value = " << value << endl;

			    cJSON_AddItemToObject(tempJsonCtrl, key, cJSON_CreateString(value));

			    if (strcmp(key, "ctrl_dev_id") == 0)
			    {
				ctrl_dev_id = value;
			    }
		    
			    /* if (strcmp(key, "act_dev_id") == 0) */
			    /* { */
			    /*     act_dev_id = value; */
			    /* } */

			   		    
			}

			if (cJSON_Array == jsonCtrlKey->type)
			{
			    cJSON_AddItemToObject(tempJsonCtrl, "actArray", jsonCtrlKey);
			    saveCtrlArrayOK = true;
			}

			if ((strcmp(ctrl_dev_id, temp2.front().dev_id) == 0) && saveCtrlArrayOK)
			{
			    cJSON_AddItemToArray(tempCtrlArray, tempJsonCtrl);
			    cout << "tempCtrlArray :" << cJSON_Print(tempCtrlArray) << endl;
			}	
		    }
		}
	    }
	    //希望通过此处循环，删除其中的"act_dev_id"字段
	    int tempCtrlArraySize = cJSON_GetArraySize(tempCtrlArray);
	    cJSON *jsonTempCtrl;
	    for (int i = 0; i < tempCtrlArraySize; i++)
	    {
		
		jsonTempCtrl = cJSON_GetArrayItem(tempCtrlArray, i);
		cJSON_DeleteItemFromObject(jsonTempCtrl, "ctrl_dev_id");
	    }

	    cJSON_AddItemToObject(dev, "contArray", tempCtrlArray);
	    cJSON_AddItemToArray(devArray, dev);

	    cout << "devArray :" << cJSON_Print(devArray) << endl;
	}

	temp2.pop_front();
    }

    cJSON_AddItemToObject(sys, "devArray", devArray);

    str = cJSON_Print(sys);

    int pos = -1;

    string oldstr = "\n";
    trim(str,oldstr, 0);
    oldstr = "\t";
    trim(str, oldstr, 0);
    oldstr = " ";
    trim(str, oldstr, 0);
    oldstr = "\r";
    trim(str, oldstr, 0);

    /* while(str.find(" ")) */
    /* { */
    /* 	pos = str.find(" "); */
    /* 	str.replace(pos, 1, ""); */
    /* } */

    
//    str = temp + str;

    
    return 0;
}



/* int dataparse::packageSignalCtrl(SrvCmd cmdStruct, string &str) */
/* { */
/*     cJSON *sys, *dev, *ctrl, *act; */
/*     cJSON *devArray, *ctrlArray, *actArray; */
/*     char *temp = 0; */

/*     sys = cJSON_CreateObject(); */
/*     dev = cJSON_CreateObject(); */
/*     ctrl = cJSON_CreateObject(); */
/*     act = cJSON_CreateObject(); */

/*     devArray = cJSON_CreateArray(); */
/*     ctrlArray = cJSON_CreateArray(); */
/*     actArray = cJSON_CreateArray(); */
 
/*     /\* Layer 1 *\/ */
/*     cJSON_AddItemToObject(sys, "sysID", cJSON_CreateString(cmdStruct.sys_id)); */
/*     temp = const_cast<char *>(intToStr(cmdStruct.ins_seq_no).c_str()); */
/*     cJSON_AddItemToObject(sys, "seqOfIns", cJSON_CreateString(temp)); */
/*     temp = const_cast<char *>(intToStr(cmdStruct.dev_no).c_str()); */
/*     cJSON_AddItemToObject(sys, "numOfDev", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(sys, "optCode", cJSON_CreateString(cmdStruct.op_code)); */
/*     cJSON_AddItemToObject(sys, "devArray", devArray); */

/*     /\* Layer 2 *\/ */
/*     cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(cmdStruct.dev_id)); */
/*     temp = const_cast<char *>(intToStr(cmdStruct.ctrl_no).c_str()); */
/*     cJSON_AddItemToObject(dev, "numOfCont", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(dev, "contArray", ctrlArray); */
/*     cJSON_AddItemToArray(devArray, dev); */

/*     /\* Layer 3 *\/ */
/*     cJSON_AddItemToObject(ctrl, "contID", cJSON_CreateString(cmdStruct.ctrl_id)); */
/*     cJSON_AddItemToObject(ctrl, "contType", cJSON_CreateString(cmdStruct.ctrl_typ)); */
/*     temp = const_cast<char *>(intToStr(cmdStruct.act_no).c_str()); */
/*     cJSON_AddItemToObject(ctrl, "numOfAct", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(ctrl, "actArray", actArray); */
/*     cJSON_AddItemToArray(ctrlArray, ctrl); */
    
/*     /\* Layer 4 *\/ */
/*     cJSON_AddItemToObject(act, "actType", cJSON_CreateString(cmdStruct.act_typ)); */
/*     cJSON_AddItemToObject(act, "actValue", cJSON_CreateString(cmdStruct.act_val)); */
/*     cJSON_AddItemToArray(actArray, act); */


/*     str = cJSON_Print(sys); */
/*     cJSON_Delete(sys); */
/*     return 0; */
/* } */



/* int dataparse::packageSignalSensor(MeteringData meterStruct, string &str) */
/* { */
/*     cJSON *sys, *dev, *sensor, *para; */
/*     cJSON *devArray, *sensorArray, *paraArray; */
/*     char *temp = 0; */

/*     sys = cJSON_CreateObject(); */
/*     dev = cJSON_CreateObject(); */
/*     sensor = cJSON_CreateObject(); */
/*     para = cJSON_CreateObject(); */

/*     devArray = cJSON_CreateArray(); */
/*     sensorArray = cJSON_CreateArray(); */
/*     paraArray = cJSON_CreateArray(); */
 
/*     /\* Layer 1 *\/ */
/*     cJSON_AddItemToObject(sys, "sysID", cJSON_CreateString(meterStruct.sys_id)); */
/*     /\* temp = const_cast<char *>(intToStr(meterStruct.insSeqNo).c_str()); *\/ */
/*     /\* cJSON_AddItemToObject(sys, "seqOfIns", cJSON_CreateString(temp)); *\/ */
/*     temp = const_cast<char *>(intToStr(meterStruct.dev_no).c_str()); */
/*     cJSON_AddItemToObject(sys, "numOfDev", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(sys, "optCode", cJSON_CreateString(meterStruct.op_code)); */
/*     cJSON_AddItemToObject(sys, "devArray", devArray); */

/*     /\* Layer 2 *\/ */
/*     cJSON_AddItemToObject(dev, "devID", cJSON_CreateString(meterStruct.dev_id)); */
/*     temp = const_cast<char *>(intToStr(meterStruct.sensor_no).c_str()); */
/*     cJSON_AddItemToObject(dev, "numOfCont", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(dev, "sensorArray", sensorArray); */
/*     cJSON_AddItemToArray(devArray, dev); */

/*     /\* Layer 3 *\/ */
/*     cJSON_AddItemToObject(sensor, "sensorID", cJSON_CreateString(meterStruct.sensor_id)); */
/*     cJSON_AddItemToObject(sensor, "sensorType", cJSON_CreateString(meterStruct.sensor_typ)); */
/*     temp = const_cast<char *>(intToStr(meterStruct.param_no).c_str()); */
/*     cJSON_AddItemToObject(sensor, "numOfPara", cJSON_CreateString(temp)); */
/*     cJSON_AddItemToObject(sensor, "paraArray", paraArray); */
/*     cJSON_AddItemToArray(sensorArray, sensor); */
    
/*     /\* Layer 4 *\/ */
/*     cJSON_AddItemToObject(para, "paraType", cJSON_CreateString(meterStruct.param_typ)); */
/*     cJSON_AddItemToObject(para, "paraValue", cJSON_CreateString(meterStruct.param_Val)); */
/*     cJSON_AddItemToObject(para, "paramValType", cJSON_CreateString(meterStruct.param_val_typ)); */
/*     cJSON_AddItemToObject(para, "paramUnit", cJSON_CreateString(meterStruct.param_unit)); */
/*     cJSON_AddItemToArray(paraArray, para); */


/*     str = cJSON_Print(sys); */
/*     cJSON_Delete(sys); */
/*     return 0; */

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
