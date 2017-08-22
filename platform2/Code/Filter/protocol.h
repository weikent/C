#ifndef __PROTOCOL_C__
#define __PROTOCOL_C__

#define START_BYTE 0XFF

#define CMD_GET_TEMP_HUMID        0X01   //温度湿度数据
#define CMD_OPEN_COOL             0X02   //打开制冷
#define CMD_OPEN_HEAT             0X03   //打开制热
#define CMD_OPEN_FAN_HIGH         0X04   //打开风机高速
#define CMD_OPEN_FAN_MIDDLE       0X05   //打开风机中速
#define CMD_OPEN_FAN_LOW          0X06   //打开风机低速
#define CMD_CLOSE_COOL            0X07   //关闭制冷
#define CMD_CLOSE_HEAT            0X08   //关闭制热
#define CMD_CLOSE_FAN             0X09  //关闭风机
#define CMD_CLOSE_ALL             0X0a  //同时关闭风机制冷制热
#define CMD_OPEN_FAN              0X0B  //纯送风


 
//#define PROTOCOL_DEBUG  //<协议数据打印

// struct message_obj{
// 	int  fd;
// 	int  delay;
// 	unsigned char message[258];
// };
// typedef struct message_obj message_t;
// void MessageInit(message_t * msg, int serial_fd, long baud_rate);
// int MessageReceive(message_t * msg);
// int  MessageSend(message_t * msg, unsigned char command, void *data, unsigned char len);
// unsigned char MessageCommand(message_t * msg);
// unsigned char MessageDataLength(message_t * msg);
// unsigned char MessageData(message_t * msg, unsigned char index);
int CheckData(const unsigned char *pData, unsigned char len);
void FetchData(unsigned char *pData, unsigned char  len);

#endif 
