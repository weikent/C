/* 
 *     Created:  20 10月 2013 17:53:20
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include "tool.h"

#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <strings.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <linux/if.h>
#include <sys/time.h>    
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <sys/wait.h>

//#include "threeDes.h"
//#include "md5.h"

#include "packMessage.h"
#include "global.h"
#include "../../common/message.h"
#include "../../common/md5.h"



#ifndef MAX_PATH
#define MAX_PATH 256
#endif

//const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


const char b64pool[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* 
 * ===  FUNCTION  ========================================================================
 *         Name: checkIsEmpty
 *  Description: 检查字符串是否为空
 *   Parameters: 
 /// @param str 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:17:54 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:17:54 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int checkIsEmpty(char *str)
{
  if (str[0] == 0 || str[0] == '\0')
    {
      return 1;
    }
  return 0;
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: GetCurrentTime
 *  Description: 获取系统当前时间
 *   Parameters: 
 /// @param time 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:18:21 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:18:21 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int GetCurrentTime(char *time)
{  
  struct timeval tv;  

  gettimeofday(&tv,NULL);

  long long time_sec = tv.tv_sec;
  int time_usec = (int)(tv.tv_usec / 1000);

  sprintf(time, "%lld%03d", time_sec, time_usec);

  return 0;
}  




/* 
 * ===  FUNCTION  ========================================================================
 *         Name: SetSystemTime
 *  Description: 设置系统时间
 *   Parameters: 
 /// @param dt 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:20:28 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:20:28 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */
/* 
 * ===  FUNCTION  ========================================================================
 *         Name: GetIP
 *  Description: 根据指定的网卡名称，获取IP地址
 *   Parameters: 
 /// @param name 
 /// @param ip 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:18:38 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:18:38 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */
int GetIP(char *name, char *ip)
{
  int s;
  struct ifconf conf;
  struct ifreq *ifr;
  char buff[BUFSIZ];
  //    char *ip;
  int num;
  int i;
  s = socket(PF_INET, SOCK_DGRAM, 0);
  conf.ifc_len = BUFSIZ;
  conf.ifc_buf = buff;
  ioctl(s, SIOCGIFCONF, &conf);
  num = conf.ifc_len / sizeof(struct ifreq);
  ifr = conf.ifc_req;
  for(i=0;i < num;i++)
    {
      struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
      ioctl(s, SIOCGIFFLAGS, ifr);
      if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
        {
          if(strcmp(ifr->ifr_name, name) == 0)
            {
              strcpy(ip, inet_ntoa(sin->sin_addr));
              close(s);
              return 0;
              //                return inet_ntoa(sin->sin_addr);
            }
          //            printf("%s (%s)\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
        }
      ifr++;
    }
  close(s);
  return 1;
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: CompareWithWavyLine
 *  Description: 判断是否是"~"
 *   Parameters: 
 /// @param buffer 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:19:16 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:19:16 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

/* int CompareWithWavyLine(char *buffer) */
/* { */
/*     char temp[10]; */
/*     bzero(temp, sizeof(temp)); */

/*     strncpy(temp, buffer, 1); */

/*     if (strcmp(temp, WAVYLINE) == 0) */
/*     { */
/*         return 0; */
/*     } */
/*     else */
/*     { */
/*         return 1; */
/*     } */
/*     return 1; */
/* } */



int SetSystemTime(char *dt)
{

  // 方法4
  /* long long lltime = atoll(dt); */

  /* long ltime = lltime / 1000; */
  /* printf ("ltime = %ld\n", ltime); */
  /* tm* ptm = localtime(&ltime); */

  /* /\* time_t tm; *\/ */
  /* /\* struct tm tm_time; *\/ */
  /* /\* time(&tm); *\/ */
  /* /\* memcpy(&tm_time, localtime(&tm), sizeof(tm_time)); *\/ */
  /* /\* tm_time.tm_sec = 44; *\/ */
  /* /\* tm_time.tm_min = 5; *\/ */
  /* /\* tm_time.tm_hour = 10; *\/ */
  /* /\* tm_time.tm_mon=8-1; *\/ */
  /* /\* tm_time.tm_year=2013-1900; *\/ */
  /* /\* tm_time.tm_mday=24; *\/ */
  /* /\* tm = mktime(&tm_time); *\/ */
  /* time_t tm; */
  /* tm = mktime(ptm); */
  /* stime(&tm); */
  /* ================================================== */


  // 方法3
  long long time = atoll(dt);
    
  //    printf ("time  = %lld\n", time);
  struct timeval settime;
  settime.tv_sec = time / 1000;
  settime.tv_usec = time % 1000;
  //    printf ("%lld\n", time % 1000);
  //    printf ("%lld\n", time / 1000);

  settimeofday(&settime, NULL);

  /* ============================================================ */

  // 方法1 
  /* char setTime[100] = ""; */
  /* sprintf(setTime, "date --date '@%ld'",time); */
  /* ============================================================ */



  // 方法2
  /* long long lltime = atoll(dt); */

  /* printf ("time = %lld\n", time); */
  /* char ctm[30]; */
  /* long ltime = lltime / 1000; */

  /* tm* ptm = localtime(&ltime); */

  /* strftime(ctm, 30, "%m%d%H%M%Y", ptm); */

  /* /\* string str = ctm; *\/ */
  /* /\* string strDate = ""; *\/ */
  /* /\* string strTime = ""; *\/ */
  /* /\* int pos = str.find(" "); *\/ */
  /* /\* if (pos != str.npos) *\/ */
  /* /\* { *\/ */
  /* /\*     strDate = str.substr(0, pos); *\/ */
  /* /\*     strTime = str.substr(pos + 1, str.length()); *\/ */
  /* /\* } *\/ */
  /* /\* else *\/ */
  /* /\* { *\/ */
  /* /\*     return -1; *\/ */
  /* /\* } *\/ */

  /* char setTime[100] = ""; */
  /* sprintf(setTime, "date -s \"%s %s\"", strDate.c_str(), strTime.c_str()); */
  /* sprintf(setTime, "date -s %s", ctm); */
  /* printf ("setTime = %s\n", setTime); */
  /* system(setTime); */

  return 0;
}





/* 
 * ===  FUNCTION  ========================================================================
 *         Name: C_Ping
 *  Description: Ping 指定的IP是否通，
 *   Parameters: 
 /// @param IP 
 * 
 * Return Value: 4次全通，返回0。非4次全通，返回1
 *      Created: Thu Apr 17 09:20:50 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:20:50 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int C_Ping(char *IP)
{
  char command[100] = "";
  sprintf(command, "ping %s -c4", IP);

  int ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {
      return 0;
    }
  else
    {
      return 1;
    }
  return 1;
}




/* 
 * ===  FUNCTION  ========================================================================
 *         Name: GetMAC
 *  Description: 根据指定的网卡名称，获取网卡的mac地址。
 *   Parameters: 
 /// @param name 网卡名称
 /// @param mac 值-value 变量。获取的mac地址存在此变量中。
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:21:38 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:21:38 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int GetMAC(char *name, char *mac)
{
  int sockfd;

  static struct ifreq req;
  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("Unable to create socket: ");
    exit(1);
  }


  strcpy(req.ifr_name, name);
  ioctl(sockfd, SIOCGIFHWADDR, &req);
  //    printf("MAC address of %s:    ",req.ifr_name);
  char tempMac[12];
  /* printf("%02X%02X%02X%02X%02X%02X", req.ifr_hwaddr.sa_data[0]&0xFF, req.ifr_hwaddr.sa_data[1]&0xFF, req.ifr_hwaddr.sa_data[2]&0xFF, req.ifr_hwaddr.sa_data[3]&0xFF, req.ifr_hwaddr.sa_data[4]&0xFF, req.ifr_hwaddr.sa_data[5]&0xFF); */ 
  sprintf(tempMac, "%02X%02X%02X%02X%02X%02X", req.ifr_hwaddr.sa_data[0]&0xFF, req.ifr_hwaddr.sa_data[1]&0xFF, req.ifr_hwaddr.sa_data[2]&0xFF, req.ifr_hwaddr.sa_data[3]&0xFF, req.ifr_hwaddr.sa_data[4]&0xFF, req.ifr_hwaddr.sa_data[5]&0xFF);

  unsigned char digest[16];
  MD5_CTX context;

  MD5Init (&context);
  MD5Update (&context, (unsigned char*)tempMac, strlen(tempMac));
  MD5Final (digest,&context);

    
  //    printf ("tempMac = %s \n", tempMac);
  int i = 0;
  for(i=0; i<16; ++i)
    {
      sprintf(tempMac + i*2, "%02X", digest[i]);
    }
  //    printf ("tempMac = %s \n", tempMac);
  strcpy(mac, tempMac);
  /* for (i = 0; i < 5; i++) { */
  /*     printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF); */
  /*     printf(":"); */
  /* } */
  /* printf("%02X", req.ifr_hwaddr.sa_data[i]&0xFF); */
  close(sockfd);
  return 0;
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: AddWaveLineAndNumInJSON
 *  Description: 在指定的json字符串中，添加“~” 和 字符串长度
 *   Parameters: 
 /// @param strJson 值-value变量
 /// @param len 字符串长度
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:23:06 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:23:06 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int AddWaveLineAndNumInJSON(char *strJson, int len)
{
  char tempStr[MAX_MSG_TEXT_SIZE + 5] = {0};
  //    tempStr = (char*)malloc(strlen(strJson) + 5);
  /* strcpy(tempStr, strJson); */
  //    memset(tempStr, 0, strlen(strJson) + 5);
  sprintf(tempStr, "~%04d", len);
  //    strcpy(strJson, tempStr);
  memcpy(tempStr + 5, strJson, len);
  memcpy(strJson, tempStr, len + 5);
  /* if (tempStr) */
  /* { */
  /*     debug_msg; */
  /*     free(tempStr); */
  /* } */

  return 0;
}


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: AddSeqNumInJSON
 *  Description: 添加seqnum和signKey。 使用seqOfIns和dt和optcode生成signkey。再添加到strJson中。
 *   Parameters: 
 /// @param strJson 
 /// @param seqOfIns 
 /// @param dt 
 /// @param optcode 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:25:43 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:25:43 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

/* int AddSeqNumInJSON(char *strJson, int seqOfIns, char *dt, char *optcode) */
/* { */
/*     int lenOfstrJson = strlen(strJson); */

/*     char char_seqOfIns[100] = {0}; */
/*     int pos = -1; */

/*     char signKey[LEN_OF_SIGNKEY] = {0}; */
/*     sprintf(signKey, "%s%d%s%s",  SYSID, seqOfIns, optcode, dt); */
/* //    printf ("signKey = %s\n", signKey); */
/*     unsigned char digest[16]; */
/*     MD5_CTX context; */

/*     MD5Init (&context); */
/*     MD5Update (&context, (unsigned char*)signKey, strlen(signKey)); */
/*     MD5Final (digest,&context); */


/*     char signKeyMd5[33] = {0}; */
/*     int i = 0; */
/*     for(i=0; i<16; ++i) */
/*     { */
/*         sprintf(signKeyMd5 + i*2, "%02X", digest[i]); */
/*         //    printf("%02X",digest[i]); */
/*     } */
/* //    printf ("signKeyMd5 = %s\n", signKeyMd5); */

    
/*     sprintf(char_seqOfIns, "\"seqOfIns\":\"%d\",\"signKey\":\"%s\",", seqOfIns, signKeyMd5); */
/*     pos = strstr(strJson, ",") - strJson; */
/*     if (pos <= 0) */
/*     { */
/*         needSendToServer = false; */
/*         return 1; */
/*     } */

/*     char *tempSeq = malloc(sizeof(char) * (lenOfstrJson + 100)); */
/*     bzero(tempSeq, (lenOfstrJson + 100)); */
/* //    char tempSeq[lenOfstrJson + 100] = {0}; */
/*     /\* tempSeq = (char*)malloc(strlen(strJson) + 100); *\/ */
/*     /\* memset(tempSeq, 0, strlen(strJson) + 100); *\/ */
/*     /\* printf ("pos = %d\n", pos); *\/ */
/*     /\* printf ("strJson = %s \n", strJson); *\/ */
/*     /\* printf ("strJson.length = %d\n", strlen(strJson)); *\/ */
/*     strncpy(tempSeq, strJson, (int)pos + 1); */
/* //    printf ("tempSeq = %s\n", tempSeq); */

/*     strcat(tempSeq, char_seqOfIns); */
/*     strcat(tempSeq, strJson + (int)pos + 1); */
/*     strcpy(strJson, tempSeq); */
/*     free(tempSeq); */

    
/*     /\* if (tempSeq) *\/ */
/*     /\* { *\/ */
/*     /\*     free(tempSeq); *\/ */
/*     /\* } *\/ */
    
/*     return 0; */
/* } */


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: GetRandomSeqOfIns
 *  Description: 获取随即的seqNum
 *   Parameters: 
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:26:41 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:26:41 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

/* int GetRandomSeqOfIns() */
/* { */
/*     struct timeval tv; */
/*     gettimeofday(&tv,NULL); */
/*     long time = tv.tv_sec * 1000 + tv.tv_usec / 1000; */
/*     int randSeed = time % RANGE_OF_SEQ_OF_INS; */

/*     srand((unsigned) randSeed); */
/*     long long lli = rand(); */
/*     int i = lli % RANGE_OF_SEQ_OF_INS; */
    
/*     i = i + 10; */
/*     return i; */
/* } */


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: ReplaceDevID
 *  Description: 替换本地层生成的字符串中的MAC地址，即devID
 *   Parameters: 
 /// @param strJson 值-value变量
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:26:57 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:26:57 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int ReplaceDevID(char *strJson)
{
  char old_str[] = "\"devID\":\"xxxxxxxxxxxx\"";
  char new_char[70] = {0};

  int pos = -1;
  sprintf(new_char, "\"devID\":\"%s\"", g_mac);
  pos = strstr(strJson, old_str) - strJson;
  if (pos <= 0)
    {
      return 1;
    }

  char tempDev[MAX_MSG_TEXT_SIZE + 70] = {0};
  /* tempDev = (char*)malloc(strlen(strJson)); */
  /* memset(tempDev, 0, strlen(strJson)); */

  strncpy(tempDev, strJson, (int)pos);
  strcat(tempDev, new_char);
  strcat(tempDev, strJson + (int)pos + sizeof(old_str) - 1);
  strcpy(strJson, tempDev);

  /* if (tempDev) */
  /* { */
  /*     free(tempDev); */
  /* } */

  return 0;
}

/* int replace_all(char *str ,const char *old_str,const char *new_str) */
/* {    */
/*     while(true) */
/*     { */
/*         char *pos = 0; */
/*         if ((pos = strstr(str, old_str)) != -1) */
/*         { */
            
/*             replace_all(str, old_str, new_str); */
/*         } */
/*         else */
/*         { */
/*             break; */
/*         } */
/*     } */
/*     //return   str;    */
/* } */



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: encry
 *  Description: 3Des加密
 *   Parameters: 
 /// @param jsonStr 值-value变量。是需要加密的字符串
 /// @param len 字符串的长度。
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:27:37 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:27:37 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

/* int encry(unsigned char *jsonStr, int len, char *key1, char *key2, char *key3) */
/* { */
/* //    unsigned char desOut[TEXT_SIZE + 8] = {0}; */
/*     unsigned char *desOut = (unsigned char*)malloc(len + 8); */
/*     memset(desOut, 0, len + 8); */
/*     unsigned char desTempOut[9] = {0}; */
/*     int i = 0; */
/*     while(1) */
/*     { */
/*         unsigned char temp[8] = {0}; */
/*         bzero(desTempOut, 9); */
/*         if ((i + 8) <= len) */
/*         { */
/*             memcpy(temp, jsonStr + i, 8); */
/* //            printf ("temp = %s\n", temp); */
  
/*             desD(temp, desTempOut, key1, key2, key3); */
/* /\*             printf ("desTempOut = %s\n", desTempOut); *\/ */
/* /\*             for (int i = 0; i < 8; ++i) *\/ */
/* /\*             { *\/ */
/* /\*                 printf ("%0X \n",desTempOut[i]); *\/ */
/* /\*             } *\/ */
/* /\* //            strncat(desOut, desTempOut, 8); *\/ */
/*             int j = 0; */
/*             for (j = 0; j < 8; j++) */
/*             { */
/*                 desOut[i+j] = desTempOut[j]; */
/*             } */

/*             /\* printf ("i = %d\n", i); *\/ */
/*             /\* for (int k = 0; k < 8; ++k) *\/ */
/*             /\* { *\/ */
/*             /\*     printf ("======%0X \n",desOut[k+i]); *\/ */
/*             /\* } *\/ */

/*             i += 8; */


/*         } */
/*         else */
/*         { */
/*             memcpy(temp, jsonStr + i, len - i); */
/*             int k = 0; */
/*             for (k = len - i; k < 8; k++) */
/*             { */
/* //                sprintf(temp + k, "%0x", 8 - (strlen(jsonStr) -i)); */
/*                 temp[k] = 8 - (len -i); */
/*             } */
/* //            printf ("temp = %s\n", temp); */
/*             desD(temp, desTempOut, key1, key2, key3); */
/* //            printf ("desTempOut = %s\n", desTempOut); */
/*             /\* for (int i = 0; i < 8; ++i) *\/ */
/*             /\* { *\/ */
/*             /\*     printf ("%0X \n",desTempOut[i]); *\/ */
/*             /\* } *\/ */
/*             int j = 0; */
/*             for (j = 0; j < 8; j++) */
/*             { */
/*                 desOut[i+j] = desTempOut[j]; */
/*             } */
/*             i += 8; */

/*             break; */
/*         } */
/*     } */

/*     memcpy(jsonStr, desOut, len + 8); */
/*     free(desOut); */
/*     return i; */
/* } */



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: decry
 *  Description: 3Des解密
 *   Parameters: 
 /// @param jsonStr 值-value变量。需要解密的字符串
 /// @param len 字符串长度
 * 
 * Return Value: 
 *      Created: Thu Apr 17 09:28:28 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Thu Apr 17 09:28:28 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

/* int decry(unsigned char *jsonStr, int len, char *key1, char *key2, char *key3) */
/* { */
/* //    unsigned char desOut[TEXT_SIZE + 8] = {0}; */
/*     unsigned char *desOut = (unsigned char*)malloc(len + 8); */
/*     unsigned char desTempOut[9] = {0}; */
/*     int i = 0; */


/*     /\* for (int i = 0; i < len; ++i) *\/ */
/*     /\* { *\/ */
/*     /\*     printf ("%02X ", jsonStr[i]); *\/ */
/*     /\* } *\/ */

/*     /\* printf (" \n"); *\/ */

/*     while(1) */
/*     { */
/*         bzero(desTempOut, 0); */
/*         unsigned char temp[8] = {0}; */

/*         if ((i + 8) <= len) */
/*         { */
/*             memcpy(temp, jsonStr + i, 8); */
/* //            i += 8; */
/*             /\* for (int i = 0; i < 8; ++i) *\/ */
/*             /\* { *\/ */
/*             /\*     printf ("%02X \n", temp[i]); *\/ */
/*             /\* } *\/ */

/*             desE(desTempOut, temp, key1, key2, key3); */

/*             //          strncat(desOut, desTempOut, 8); */
/*             memcpy(desOut + i, desTempOut, 8); */
/*             i += 8; */
/*         } */
/*         else */
/*         { */
/*             break; */
/*         } */
/* //        printf ("desTempOut = %s\n", desTempOut); */
/*     } */
/* //    printf("desOut = %s \n", desOut); */


/* //    strcpy(jsonStr, desOut); */
/*     memcpy(jsonStr, desOut, len); */
/*     free(desOut); */
/*     return 0; */
/* } */


int isRightIP(const char *str)
{
  struct in_addr addr;
  int ret;
  volatile int local_errno;

  errno = 0;
  ret = inet_pton(AF_INET, str, &addr);
  /* local_errno = errno; */
  /* if (ret > 0) */
  /*     fprintf(stderr, "\"%s\" is a valid IPv4 address\n", str); */
  /* else if (ret < 0) */
  /*     fprintf(stderr, "EAFNOSUPPORT: %s\n", strerror(local_errno)); */
  /* else  */
  /*     fprintf(stderr, "\"%s\" is not a valid IPv4 address\n", str); */

  return ret;
}

/* 
 * ===  FUNCTION  ========================================================================
 *         Name: CloseTCPConnection
 *  Description: 关闭socket。
 *               重置clientModelTCPSocket = -1;
 *               如果当前连接数过大，则重启设备
 *               
 *   Parameters: 
 * 
 * Return Value: 
 *      Created: Wed Apr 16 11:09:02 2014 by世建 魏
 *     Revision: 
 * =======================================================================================
 * @0.1   世建 魏  Wed Apr 16 11:09:02 2014 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int CloseTCPConnection()
{
  debug_msg("--");
  if (clientModelTCPSocket > 1000)
    {
      system("reboot");
    }

  debug_msg("--");
  if (clientModelTCPSocket == -1)
    {
      return 0;
    }
  debug_msg("--");
  close(clientModelTCPSocket);

  debug_msg("after close\n");

  clientModelTCPSocket = -1;

  g_isCreated = false;
  g_isConnected = false;
  g_isSubscribed = false;
  return 0;
}



/* 
 * ===  FUNCTION  ========================================================================
 *         Name: addTopicList
 *  Description: 增加一个topic到topicList中
 *   Parameters: 
 /// @param topic  需要增加的字符串
 * 
 * Return Value: 0:成功
 *               1:失败
 *      Created: Thu Jul 30 13:06:50 2015 byweishijian
 *     Revision: 
 * =======================================================================================
 * @0.1   weishijian  Thu Jul 30 13:06:50 2015 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int addTopicList(char *topic)
{
  if (topicListCount == 0) {
    topicListCount ++;
    topicList = (char **)malloc(sizeof(char *) * topicListCount);
    if (topicList == NULL) {
      /* TODO: */
      system("reboot");
    } else {
      *(topicList + (topicListCount - 1)) = topic;
      return 0;
    }
  } else {
    int i = 0;
    _Bool isExisted = false;

    for (i = 0; i < topicListCount; i++) {
      if (strcmp(*topicList + i, topic) == 0) { /* the new topic has alread added to the topicList */
        isExisted = true;
        return 0;                                /* so break this loop, and return */
      } else {
            
      }         
    }

    if (!isExisted) {
      topicListCount ++;
      char **temp = realloc(topicList, sizeof(char *) * topicListCount);
      if (temp == NULL) {
        /* TODO:  */
        system("reboot");
      } else {
        *(topicList + (topicListCount - 1)) = topic;
      }         
    } else {
    }
  }
  return 0;
}


int getCommandResult(char *command, char **buf){

  FILE   *stream; 

  stream = popen(command, "r");

  fread( *buf, sizeof(char), sizeof(*buf), stream);

  printf("stream = %s", *buf);
  pclose( stream );
  return 0;
}




// char * base64_encode( const unsigned char * bindata, char * base64, int binlength )
// {
//     int i, j;
//     unsigned char current;

//     for ( i = 0, j = 0 ; i < binlength ; i += 3 )
//     {
//         current = (bindata[i] >> 2) ;
//         current &= (unsigned char)0x3F;
//         base64[j++] = base64char[(int)current];

//         current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
//         if ( i + 1 >= binlength )
//         {
//             base64[j++] = base64char[(int)current];
//             base64[j++] = '=';
//             base64[j++] = '=';
//             break;
//         }
//         current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
//         base64[j++] = base64char[(int)current];

//         current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
//         if ( i + 2 >= binlength )
//         {
//             base64[j++] = base64char[(int)current];
//             base64[j++] = '=';
//             break;
//         }
//         current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
//         base64[j++] = base64char[(int)current];

//         current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
//         base64[j++] = base64char[(int)current];
//     }
//     base64[j] = '\0';
//     return base64;
// }


// int base64_decode( const char * base64, unsigned char * bindata )
// {
//     int i, j;
//     unsigned char k;
//     unsigned char temp[4];
//     for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
//     {
//         memset( temp, 0xFF, sizeof(temp) );
//         for ( k = 0 ; k < 64 ; k ++ )
//         {
//             if ( base64char[k] == base64[i] )
//                 temp[0]= k;
//         }
//         for ( k = 0 ; k < 64 ; k ++ )
//         {
//             if ( base64char[k] == base64[i+1] )
//                 temp[1]= k;
//         }
//         for ( k = 0 ; k < 64 ; k ++ )
//         {
//             if ( base64char[k] == base64[i+2] )
//                 temp[2]= k;
//         }
//         for ( k = 0 ; k < 64 ; k ++ )
//         {
//             if ( base64char[k] == base64[i+3] )
//                 temp[3]= k;
//         }

//         bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
//             ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
//         if ( base64[i+2] == '=' )
//             break;

//         bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
//             ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
//         if ( base64[i+3] == '=' )
//             break;

//         bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
//             ((unsigned char)(temp[3]&0x3F));
//     }
//     return j;
// }






unsigned long b64decode(char* sz, void* pv)
{
  int i,j;
  char c[4],*pc=(char*)pv;
  for (;;pc++) {
    for (i=0;i<4&&*sz;sz++) {
      switch (*sz) {
      case '=': c[i++]=0;
      case '\r':
      case '\n': continue;
      default:
        for (j=0;j<64;j++)
          if (*sz==b64pool[j]) break;
        c[i++]=j;
      }
    }
    if (i>0) *pc=c[0]<<2|c[1]>>4;
    if (i>1) *++pc=c[1]<<4|c[2]>>2;
    if (i>2) *++pc=c[2]<<6|c[3];
    if (i<4) break;
  }
  return pc-(char*)pv;
}

unsigned long b64encode(char* sz, void* pv, unsigned long cb, int bk)
{
  unsigned long i,j;
  unsigned char b[3],*pb=(unsigned char*)pv;

  for (j=0;;) {
    b[0]=b[1]=b[2]=0;
    for (i=0;i<3&&cb>0;i++,cb--)
      b[i]=*pb++;
    if (i==0) break;
    *sz++=b64pool[b[0]>>2];
    *sz++=b64pool[(b[0]&0x03)<<4|b[1]>>4];
    *sz++=(i>1)?b64pool[(b[1]&0x0f)<<2|b[2]>>6]:'=';
    *sz++=(i>2)?b64pool[b[2]&0x3f]:'=';
    j+=4;
    if (bk&&j%76==0) { *sz++='\r'; *sz++='\n'; }
  }
  *sz=0;
  return j;
}

/* int startech_base64decode(int len, const char *data, char *result) */
/* { */
/*     unsigned long cbBytes; */
/*     unsigned char *pbData; */
/*     char equalChar = 0; */
/*     if (data[lenOfActualData - 1] == 0x3D && data[lenOfActualData - 2] == 0x3D) { */
/*         cbBytes = (lenOfActualData - 2) * 3/4; */
/*         equalChar = 2; */
/*     }else if (data[lenOfActualData - 1] == 0x3D && data[lenOfActualData - 2] != 0x3D) { */
/*         cbBytes = (lenOfActualData - 1) * 3/4; */
/*         equalChar = 1; */
/*     }else if (data[lenOfActualData - 1] != 0x3D && data[lenOfActualData - 2] != 0x3D) { */
/*         cbBytes = (lenOfActualData) * 3/4; */
/*         equalChar = 0; */
/*     }else{ */
/*         cbBytes = 0; */
/*     } */

/*     debug_msg("cbBytes = %d\n", cbBytes); */

/*     pbData = (unsigned char*)malloc(cbBytes + 1); */
/*     bzero(pbData, cbBytes + 1); */

/*     cbBytes = b64decode(data, pbData); */
/*     debug_msg("base64 decoded = %s\n", pbData); */
/*     debug_msg("cbBytes = %d\n", cbBytes); */

/*     cbBytes -= equalChar; */

/*     result = malloc(cbBytes + 1); */
/*     memcpy(result, pbData, cbBytes); */
/*     free(pbData); */
/*     int i = 0; */
/*     for (i = 0; i < cbBytes; i++) { */
/*         printf("%02X ", pbData[i]); */
/*     } */
/*     printf("\n"); */

/*     return cbBytes; */
/* } */

/* int startech_base64encode(int len, const char *data, char *result) */
/* { */
/*     bool bbreakLine=false; */
/*     unsigned long cbB64Len, cbBytes; */
/*     char *szBase64; */

/*     cbBytes = len; */

/* //計算所需記憶體空間 */
/*     cbB64Len = (cbBytes+2)/3*4+1; */
/*     if (bbreakLine) cbB64Len += cbB64Len/76*2; */
/* //配置記憶體空間 */
/*     szBase64 = (char*)malloc(cbB64Len); */
/* //呼叫編碼參數說明:szBase64=編碼結果;pbData=原始資料; */
/* //cbBytes=資料長度;bbreakLine=自動換行(true/false); */
/* //若bbreakLine設為true則每76個字即插入換行符號 */
/* //若為false則輸出不換行之連續長字串 */
/*     int base64Len= b64encode(szBase64, data, cbBytes, bbreakLine); */

/*     result = malloc(base64Len + 1); */
/*     memcpy(result, szBase64, base64Len); */

/*     free(szBase64); */
/*     // printf("\nbase64 encoded:\n%s\n", szBase64); */
/*     // printf("cbB64len = %d\njj = %d\n", cbB64Len, base64Len); */
/*     // int i = 0; */
/*     // for (i = 0; i < base64Len; i++) { */
/*     //     printf("%02X,", szBase64[i]); */
/*     // }; */
/*     // printf("\n" ); */

/*     return base64Len; */
/* } */


/* 
 * ===  FUNCTION  ========================================================================
 *         Name: mqttPublish
 *  Description: 生成一个mqtt格式的发送包
 *   Parameters:
 /// @param int data的长度  
 /// @param data 需要发送的数据
 /// @param mqttData 生成的mqtt格式包
 * 
 * Return Value: 
 *      Created: Tue Apr 12 14:27:33 2016 byweishijian
 *     Revision: 
 * =======================================================================================
 * @0.1   weishijian  Tue Apr 12 14:27:33 2016 , create orignal file
 * =======================================================================================
 * Copyright (c), DingQing Technology, Ltd.
 * =======================================================================================
 */

int mqttPublish(int len, const char *data, char **mqttData)
{

  struct mqttSend *mqtt = malloc(sizeof(struct mqttSend));

  /* set message type */
  mqtt->fixedHeader.messageType = PUBLISH;

  /* set topic filter */
  int topicFilterLen = 0;
  unsigned char *topicFilter = NULL;

  mqtt->fixedHeader.Qos = 0x00;
  mqtt->fixedHeader.DUP = 0x00;
  mqtt->fixedHeader.REATIN = 0x00;

  /* 此处用于判断是设备主动上传的，还是设备在收到指令后做出的响应。 */

  topicFilterLen = strlen(PREFIX_PUBLISH_TOPIC_FILTER) + strlen(g_mac);
  topicFilter = (unsigned char *)malloc(sizeof(char) * (topicFilterLen + 1));    
  bzero(topicFilter, topicFilterLen + 1);
  memcpy(topicFilter, PREFIX_PUBLISH_TOPIC_FILTER, LEN_OF_PREFIX_PUBLISH_TOPIC_FILTER);
  memcpy(topicFilter + LEN_OF_PREFIX_PUBLISH_TOPIC_FILTER, g_mac, strlen(g_mac));


  mqtt->variableHeader.topicFilterArr = (char**)malloc(1 * sizeof(char*));
  mqtt->variableHeader.i_topicFilterArrCount = 1;
  //        mqtt->variableHeader.packetIdentifer = seqOfIns;


  *(mqtt->variableHeader.topicFilterArr) = malloc(topicFilterLen + 1);
  bzero(*(mqtt->variableHeader.topicFilterArr), topicFilterLen + 1);
  memcpy(*(mqtt->variableHeader.topicFilterArr), topicFilter, topicFilterLen);

  free(topicFilter);


  mqtt->playLoad.actualData = malloc(len + 1);
  bzero(mqtt->playLoad.actualData, len + 1);
  memcpy(mqtt->playLoad.actualData, data, len);

  mqtt->playLoad.i_actualDataLen = len;

    
  unsigned char *temp = NULL;
  int publishLen = packMessage(mqtt, &temp);

  //    int i = 0;
  // for (i = 0; i < publishLen; i++) {
  //     printf("%02X,", temp[i]);
  // }
  // printf("\n");

  *mqttData = malloc(publishLen + 1);
  memcpy(*mqttData, temp, publishLen);
  // for (i = 0; i < publishLen; i++) {
  //     printf("%02X,", (*mqttData)[i]);
  // }
  // printf("\n");

  free(mqtt->playLoad.actualData);
  free(*(mqtt->variableHeader.topicFilterArr));
  free(mqtt->variableHeader.topicFilterArr);
  free(mqtt);

  free(temp);
  return publishLen;
}


int setDhcpInfo()
{
  char setIP[100] = {0};
  int ret = -1;

  sprintf(setIP, "ifconfig ra0 %s netmask %s up", g_ip_addr, g_mask);
  debug_msg("setIP = %s\n", setIP);
  ret = system(setIP);
  if (WEXITSTATUS(ret) == 0) {
  }else{
    return 1;
  }
  sleep(1);

  char setGW[100] = {0};
  sprintf(setGW, "route add default gw %s",g_gateWay);
  debug_msg("setGW = %s\n", setGW);
  ret = system(setGW);
  if (WEXITSTATUS(ret) == 0) {
  }else{
    return 1;
  }
  sleep(1);

  system("rm /etc/resolv.conf");
  system("touch /etc/resolv.conf");
  char adddns1[100] = {0};
  sprintf(adddns1, "echo nameserver %s >> /etc/resolv.conf", g_dns1);
  char adddns2[100] = {0};
  sprintf(adddns2, "echo nameserver %s >> /etc/resolv.conf", g_dns2);
  ret = system(adddns1);
  if (WEXITSTATUS(ret) == 0) {
  }else{
    return 1;
  }
  sleep(1);

  ret = system(adddns2);
  if (WEXITSTATUS(ret) == 0) {
  }else{
    return 1;
  }

  sleep(3);
  return 0;
}

/**
 * change 7688 to station model
 *
 * @param 
 * @return
 * 0: change successed.
 * 1: change failed.
 */
int changeToSta(){
  char command[100] = {0};
  int ret = -1;

  bzero(command, 100);
  sprintf(command, "uci set wireless.sta.ssid=%s", g_ssid);
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);

  sprintf(command, "uci set wireless.sta.key=%s", g_password);
  ret = system(command);
  if (WEXITSTATUS(ret) == 0) {
    
  }else{
    return 1;
  }
  bzero(command, 100);

  sprintf(command, "uci set wireless.sta.encryption=%s", "psk2");
  ret = system(command);
  if (WEXITSTATUS(ret) == 0) {
    
  }else{
    return 1;
  }
  bzero(command, 100);

  sprintf(command, "uci set wireless.sta.disabled=0");
  ret = system(command);
  if (WEXITSTATUS(ret) == 0) {
    
  }else{
    return 1;
  }
  bzero(command, 100);

  sprintf(command, "uci commit");
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);

  sprintf(command, "wifi");
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);
  return 0;

  return 0;
}



/**
 * change 7688 to AP model
 *
 * @param 
 * @return <ReturnValue>
 * 0: change success
 * 1: change failed
 */
int changeToAP(){
  char command[100] = {0};
  int ret = -1;
  bzero(command, 100);
  sprintf(command, "uci set wireless.sta.disabled=1");
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);

  sprintf(command, "uci commit");
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);

  sprintf(command, "wifi");
  ret = system(command);
  if (WEXITSTATUS( ret ) == 0)
    {}
  else
    {
      return 1;
    }
  bzero(command, 100);
  return 0;
}
