// -*- C++ -*-

/* @(#)tool.h
 */

#ifndef _TOOL_H
#define _TOOL_H 1

#ifdef __cplusplus
extern "C"
{
#endif

    extern int GetCurrentTime(char *time);
    extern int GetIP(char *name, char *ip);
    extern int CompareWithWavyLine(char *buffer);
    extern int SetSystemTime(char *dt);
    extern int C_Ping(char *IP);
    extern int GetMAC(char *name, char *mac);
    extern int AddWaveLineAndNumInJSON(char *strJson, int len);
//    extern int AddWaveLineAndNumInJSON(char *strJson);
    extern int AddSeqNumInJSON(char *strJson, int seqOfIns, char *dt, char *optcode);
//    extern int AddSeqNumInJSON(char *strJson, int seqOfIns);    
    extern int GetRandomSeqOfIns();
    extern int ReplaceDevID(char *strJson);
    extern int checkIsEmpty(char *str);
    // extern int decry(unsigned char *jsonStr, int len);
    // extern int encry(unsigned char *jsonStr,int len);
    extern int encry(unsigned char *jsonStr, int len, char *key1, char *key2, char *key3);
    extern int decry(unsigned char *jsonStr, int len, char *key1, char *key2, char *key3);
    extern int isRightIP(const char *str);

    extern int CloseTCPConnection();
    extern int CloseTCPConnectionAPI();
    extern int addTopicList(char *topic);
    extern unsigned long b64decode(char* sz, void* pv);
    extern unsigned long b64encode(char* sz, void* pv, unsigned long cb, int bk);
    // extern int startech_base64decode(int len, const char *data, char *result);
    // extern int startech_base64encode(int len, const char *data, char *result);
    extern int mqttPublish(int len, const char *data, char **mqttData);
#ifdef __cplusplus
}
#endif

#endif /* _TOOL_H */

