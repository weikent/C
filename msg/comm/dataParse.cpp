/* 
 *     Created:  02 Jul 2013 20:22:30
 *     Author:   root@UbuntuMac
 Description:  
*/

//#include <config.h>
#include "dataParse.h"
#include "cJSON.h"
#include <cstdio>
#include <cstring>



dataParse::dataParse()
{
}
dataParse::~dataParse()
{
}

int dataParse::serverDataParse(const char *serverData)
{
    char *out;cJSON *json;
	
    json=cJSON_Parse(serverData);

    if (json)
    {

	printf ("%d\n", json->type);
	printf ("%d\n", json->valueint);
	printf ("%d\n", cJSON_GetArraySize(json));

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
//	    printf("jsonSys = %d", jsonSys);

	    if (NULL == jsonSys)
	    {
		
	    }

	    printf ("jsonSys->type = %d\n", jsonSys->type);

	    if (cJSON_Number == jsonSys->type)
	    {
		printf("cJson_number");
		printf ("%d\n", jsonSys->valueint);
	    }
	
	    if (cJSON_String == jsonSys->type)
	    {
		printf("cjson_string");
		printf ("%s\n", jsonSys->valuestring);

		key = jsonSys->string;
		value = jsonSys->valuestring;

		printf ("key = %s\n",key);
		if (strcmp(key, "sysID") == 0)
		{
		    printf ("sysID Value = %s\n", value);		    
		}
		if (strcmp(key, "timeout") == 0)
		{
		    printf ("timeout value = %s\n", value);		    
		}
		if (strcmp(key, "optCode") == 0)
		{
		    printf("optCode value = %s\n", value);		    
		}
		if (strcmp(key, "numOfDev") ==0)
		{
		    printf("numOfDev value = %s\n", value);
		}
	    }


	    if (cJSON_Array == jsonSys->type)
	    {
		printf("\t cjson_array    Dev\n");
		int n_dev = cJSON_GetArraySize(jsonSys);
		int i_dev = 0;
		printf ("cjson_array Dev size is %d\n", n_dev);


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
				    printf("\t devID value = %s\n", value);
				}

				if (strcmp(key, "numOfCont") == 0)
				{
				    printf ("\t numOfCont value = %s\n", value);
				}
				
			    }

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
						    printf ("\t\t contID value = %s\n", value);
						}
						
						if (strcmp(key, "contType") == 0)
						{
						    printf ("\t\t contType value = %s\n", value);
						}

						if (strcmp(key, "numOfAct") == 0)
						{
						    printf("\t\t numOfAct value = %s\n", value);
						}
					    }

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
								if (strcmp(key, "actID") == 0)
								{
								    printf ("\t\t\t actID value = %s\n", value);
								}
						
								if (strcmp(key, "actValue") == 0)
								{
								    printf ("\t\t\t actValue value = %s\n", value);
								}
							    }
							}
//							cJSON_Delete(jsonActOrParaKey);
						    }
						}
					    }
					}
//					cJSON_Delete(jsonContOrSenKey);
				    }
				}
			    }
			}
		    }
		}
	    }

	}

//	printf ("%s\n", json->valuestring);
//	printf ("%s\n", json->string );
//	printf ("%lf\n", json->valuedouble);
	    


//	out=cJSON_Print(json);
//	cJSON_Delete(json);
	/* cJSON_Delete(jsonSys); */
	/* cJSON_Delete(jsonDev); */
	/* cJSON_Delete(jsonContOrSen); */
	/* cJSON_Delete(jsonActOrPara); */
//	printf("%s\n",out);
//	free(out);

    }
    else
    {
	printf ("error parse [%s]\n", cJSON_GetErrorPtr());
	return -1;
    }
    return 0;

}
