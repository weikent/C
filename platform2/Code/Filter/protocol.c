#include <memory.h>
#include <unistd.h>
#include "protocol.h"
#include <stdio.h>





int dealTemAndHum(const unsigned char *pData )
{
  return 0;
}

int dealOpenCool(const unsigned char *pData )
{
  return 0;
}

int dealOpenHot(const unsigned char *pData )
{
  return 0;
}

int dealOpenFanHigh(const unsigned char *pData )
{
  return 0;
}

int dealOpenFanMiddle(const unsigned char *pData )
{
  return 0;
}

int dealOpenFanLow(const unsigned char *pData )
{
  return 0;
}

int dealCloseCool(const unsigned char *pData )
{
  return 0;
}

int dealCloseHot(const unsigned char *pData )
{
  return 0;
}

int dealCloseFan(const unsigned char *pData )
{
  return 0;
}

int dealCloseAll(const unsigned char *pData )
{
  return 0;
}

int dealOpenFan(const unsigned char *pData )
{
  return 0;
}

struct operation{

  int cmdid; 
  int(* fun)(const unsigned char *pData);
};

struct operation operations[] = {
  {1, dealTemAndHum},
  {2, dealOpenCool},
  {3, dealOpenHot},
  {4, dealOpenFanHigh},
  {5, dealOpenFanMiddle},
  {6, dealOpenFanLow},
  {7, dealCloseCool},
  {8, dealCloseHot},
  {9, dealCloseFan},
  {10, dealCloseAll},
  {11, dealOpenFan}
};


const int operationCount = sizeof(operations)/sizeof(struct operation);


int CheckData(const unsigned char *pData, unsigned char len)
{
  unsigned char sm = 0;
  unsigned char xr = 0;
  while (len--)
  {
    sm += *pData;
    xr ^= *pData;
    pData++;
  }
  sm ^= 0x55;
  xr ^= 0xAA;
  return (((sm==*pData) && (xr==*(++pData)))? 1:0);
}

void FetchData(unsigned char *pData, unsigned char  len)
{
  unsigned char sm = 0;
  unsigned char xr = 0;
  while (len--)
  {
    sm += *pData;
    xr ^= *pData;
    pData++;
  }
  *pData  = sm ^ 0x55;
  pData++;
  *pData  = xr ^ 0xAA;
}


int ParseData(const unsigned char *pData, unsigned char len)
{
  if (*pData == START_BYTE) {
    char cmd = *(++pData);
    int i = 0;
    for (i = 0; i < operationCount; i++) {
      if (operations[i].cmdid == cmd) {
        operations->fun(++pData);
      }
    }


    /* if (cmd == CMD_GET_TEMP_HUMID) { */
    /*   char data = *(pData++); */
    /*   if (data == 2) { */
    /*     char temperature = *(pData++); */
    /*     char humidity = *(pData ++); */
    /*   }else{ */
    /*     return -1; */
    /*   } */
    /* }else if (cmd == CMD_OPEN_COOL) { */
    /* }else if (cmd == CMD_OPEN_HEAT) { */
    /* }else if (cmd == CMD_OPEN_FAN_HIGH){ */
    /* }else if (cmd == CMD_OPEN_FAN_MIDDLE){ */
    /* }else if (cmd == CMD_OPEN_FAN_LOW) { */
    /* }else if (cmd == CMD_CLOSE_COOL) { */
    /* }else if (cmd == CMD_CLOSE_HEAT) { */
    /* }else if (cmd == CMD_CLOSE_FAN) { */
    /* }else if (cmd == CMD_CLOSE_ALL) { */
    /* }else if (cmd == CMD_OPEN_FAN) { */
    /* }else{ */
    /*   return -1; */
    /* } */
    return 0;
  }else{
    return -1;
  }
}
