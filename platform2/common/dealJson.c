/* 
 *     Created:  02 12月 2013 13:29:14
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include "dealJson.h"

//#include "createFile.h"
#include "cJSON.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int isExist(char *filename)
{
  return (access(filename,0) == 0);
}


/************/
/* 0:ok     */
/* 1:failed */
/************/
// configFile should be a full path.
int getValueFromJson(char *configFile, char *key, char *value)
{
    if (isExist(configFile))
    {}
    else
    {
      //        createFile(configFile);
      return 1;
    }

    FILE *f=fopen(configFile,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
    char *data=(char*)malloc(len + 1);fread(data,1,len,f);fclose(f);

    cJSON *root = cJSON_Parse(data);

    if (root)
    {
        int n_Key = cJSON_GetArraySize(root);
        int i_Key = 0;

        cJSON *jsonItem;

        char *jsonKey;
        char *jsonValue;

        for (i_Key = 0; i_Key < n_Key; i_Key++)
        {
            jsonItem = cJSON_GetArrayItem(root, i_Key);

            if (cJSON_String == jsonItem->type)
            {
                jsonKey = jsonItem->string;
                jsonValue = jsonItem->valuestring;
                if (strcmp(jsonKey, key) == 0)
                {
                    /* printf ("jsonValue = %s\n", jsonValue); */
                    /* printf ("jsonKey = %s\n", jsonKey); */
                    strcpy(value, jsonValue);
                    free(data);
                    cJSON_Delete(root);
                    return 0;
                }
            }
        }
    }
    else
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }

    cJSON_Delete(root);
    free(data);
    return 1;
}

/************/
/* 0:ok     */
/* 1:failed */
/************/
// configFile should be a full path.
int setValueToJson(char *configFile, char *key, char *value)
{
    if (isExist(configFile))
    {}
    else
    {
      //        createFile(configFile);
      return 1;
    }
    

    FILE *f=fopen(configFile,"rb");fseek(f,0,SEEK_END);long len=ftell(f);fseek(f,0,SEEK_SET);
    char *data=(char*)malloc(len + 1);fread(data,1,len,f);fclose(f);


    cJSON *root = cJSON_Parse(data);
    

    if (root)
    {
        char tempValue[200] = {0};

        if (getValueFromJson(configFile, key, tempValue) == 0)
        {
            cJSON_ReplaceItemInObject(root, key, cJSON_CreateString(value));
        }
        else
        {
            cJSON_AddItemToObject(root, key, cJSON_CreateString(value));
        }

        char *fileData = cJSON_Print(root);
    
        remove(configFile);
        FILE *file = fopen(configFile, "w");
        rewind(file);
        fprintf(file, "%s\n", fileData);
        fclose(file);
        free(fileData);
        free(data);
        cJSON_Delete(root);

        return 0;

    }
    else
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    }

    cJSON_Delete(root);
    free(data);
    return 1;
}

