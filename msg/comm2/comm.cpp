/* 
 *     Created:  15 May 2013 14:45:57
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
/* #include "getOtherID.h" */
/* #include "msgqueue.h" */


#include <iostream>
#include <string>
#include <ctype.h>
#include <sys/types.h>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <csignal>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
//#include <linux/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include "msgsnd_cmd.h"
#include "msgrcv_data.h"
#include "dataparse.h"

#include "global.h"


//for switch AP/Client 
#include "common.h" 
#include "wpa_supplicant_i.h" 
#include "customopt.h" 
#include "wifitypes.h" 
#include "apoperation.h" 
#include "APModel.h"


#include "utility.h"

#include "msgqueue.h"
//#include "global/global.h"


using namespace std;

#define MAX_DATA_SEND_TO_SERVER 1024


utility utility;
msgqueue msg;
int message_t;  //消息队列

//int localID;   //本地进程ID
//struct mymsgbuf qbuf;   // 消息队列的结构体，用来保存消息队列中的信息。


/* int msg_qid; */
/* key_t key; */

dataparse dp;
APModel apmodel;
int connectModel = 0;   // 0: 短连接    1:长连接   -1:程序首次启动
int connectIsOK = 0;   //判断连接是否成功 长链接时使用，短连接时一般不判断此值
/* int sameLAN = 0;    //0:不在同一个LAN   1:在同一个LAN */

bool hasDataToSend = false; // 本地层是不是有信息要发到服务器。


pthread_t thread[3];  // thread1: sendHeart ,  thread2: localToServer   thread3: serverToLocal


int socketTOServer = -1;  //与服务器连接的socket描述符。

char buffer[MAX_DATA_SEND_TO_SERVER]={0};    //接收与发送的缓冲区。

//char serverIP[] = "192.168.5.74";
//int serverPort = 14567;
extern char serverIP[15];
extern int serverPort;




Msg_recv* msgrecv_thrd = NULL;



void *sendHeart(void *arg);
void filterData(const char *data);
int changeConnetModel(char *buff);
void closeConnect();
void sendmessageToLocal();
void *serverToLocal(void *arg);


extern enum deviceModel deviceModel;

/* 过滤数据，检查是否需要通讯层处理 */
void filterData(const char *data)
{
    char *optcode = dp.checkOptCode(data);

    char temp[10];
    bzero(temp, sizeof(temp));
    strncpy(temp, optcode, 4);
    printf ("%s\n",temp);
    if (strcmp(temp, "0080") == 0)
    {
	printf ("change connect mode to short connection!\n", 0);

	closeConnect();
	connectModel = 0;
	connectIsOK = 0;
    }
    else if (strcmp(temp, "0070") == 0)
    {
	int threadok = -1;
	printf ("change connect mode to long connection!\n", 1);
	closeConnect();
	connectModel = 1;
	if ((threadok = pthread_create(&thread[2], NULL, serverToLocal, NULL)) != 0)
	{
	    printf ("create thread for serverToLocal failed !\n");
	}
	else
	{
	    printf ("create thread for serverToLocal successed !\n");
	}

	// 发送心跳
	if((threadok = pthread_create(&thread[0], NULL, sendHeart, NULL)) != 0)
	    printf("create thread for send heart failed !\n");
	else
	    printf("create thread for send heart successed !\n");
    }
    else
    {

	if (dp.parse(buffer) == -1)
	{
			
	}
	else
	{

	    sendmessageToLocal();
	}
    }
}


void closeConnect()
{
    close(socketTOServer);
    socketTOServer = -1;
}


string& trim(string &str, string oldstr,string::size_type pos = 0)
{
    string delim = oldstr; //删除空格或者tab字符
    pos = str.find_first_of(delim, pos);
    if (pos == string::npos)
        return str;
    return trim(str.erase(pos, 1), delim);
}


/********************************************************/
/* 检查从服务器接收的数据，是不是要改变程序的连接模式。 */
/* 即设置长/短连接				        */
/********************************************************/
int changeConnetModel(char *buff)
{
    char temp[10];

    bzero(temp, sizeof(temp));
    strncpy(temp, buff, 4);
    printf ("%s\n",temp);
    if (strcmp(temp, "0080") == 0)
    {
	printf ("change connect mode to short connection!\n", 0);
	closeConnect();
	return 0;
    }
    else if (strcmp(temp, "0070") == 0)
    {
	printf ("change connect mode to long connection!\n", 1);
	return 1;
    }
    else
    {
	closeConnect();
	printf ("change connect mode to short connection!\n", 0);
	return 0;
    }
}

void destroy_msgrecv_thrd () {
    msgrecv_thrd->stop();
    cout << "msgrecv_thrd->stop() is called." << endl;
    delete msgrecv_thrd;
}


void sendmessageToLocal()
{
    /* cout << "sending...." << endl; */
    Msg_send msgsnd("tpt_snd_ctl_rcv");

    size_t sizMsg = sizeof(SrvCmd)+sizeof(long);
    MsgCmd tMsgCmd;


    /* usleep(182000); */
    /* printf ("dp.cmdQueue.size() = %d \n", dp.cmdQueue.size()); */

    while(dp.cmdQueue.size())
    {
	/* printf ("pop_front()\n"); */
	tMsgCmd.msg_type = sizMsg;
	strcpy(tMsgCmd.msg_text.sys_id, dp.cmdQueue.front().sys_id);
	tMsgCmd.msg_text.ins_seq_no = dp.cmdQueue.front().ins_seq_no;
	tMsgCmd.msg_text.dev_no = dp.cmdQueue.front().dev_no;
	strcpy(tMsgCmd.msg_text.op_code, dp.cmdQueue.front().op_code);
	strcpy(tMsgCmd.msg_text.dev_id, dp.cmdQueue.front().dev_id);
	tMsgCmd.msg_text.ctrl_no = dp.cmdQueue.front().ctrl_no;

	strcpy(tMsgCmd.msg_text.ctrl_id, dp.cmdQueue.front().ctrl_id);
	strcpy(tMsgCmd.msg_text.ctrl_typ, dp.cmdQueue.front().ctrl_typ);
	tMsgCmd.msg_text.act_no = dp.cmdQueue.front().act_no;
	
	strcpy(tMsgCmd.msg_text.act_typ, dp.cmdQueue.front().act_typ);
	strcpy(tMsgCmd.msg_text.act_val, dp.cmdQueue.front().act_val);
	strcpy(tMsgCmd.msg_text.act_unit, dp.cmdQueue.front().act_unit);
	tMsgCmd.msg_text.act_time, dp.cmdQueue.front().act_time;
	tMsgCmd.msg_text.act_precs = dp.cmdQueue.front().act_precs;
	tMsgCmd.msg_text.act_min = dp.cmdQueue.front().act_min;
	tMsgCmd.msg_text.act_max = dp.cmdQueue.front().act_max;
	strcpy(tMsgCmd.msg_text.act_stat, dp.cmdQueue.front().act_stat);
	tMsgCmd.msg_text.act_stat_time = dp.cmdQueue.front().act_stat_time;

	//用来测试
	/* string str; */
	/* dp.packageSignalCtrl(dp.cmdQueue.front(), str); */
	/* cout << str << endl; */


	/* ========== */
	cout << tMsgCmd.msg_type << endl;
	cout << tMsgCmd.msg_text.sys_id << ": ";
	cout << tMsgCmd.msg_text.ins_seq_no << ": ";
	cout << tMsgCmd.msg_text.dev_no << ": ";
	cout << tMsgCmd.msg_text.op_code << ": ";
	cout << tMsgCmd.msg_text.dev_id << ": ";
	cout << tMsgCmd.msg_text.ctrl_no << ": ";
	cout << tMsgCmd.msg_text.ctrl_id << ": ";
	cout << tMsgCmd.msg_text.ctrl_typ << ": ";
	cout << tMsgCmd.msg_text.act_no << ": ";
	cout << tMsgCmd.msg_text.act_typ << ": ";
	cout << tMsgCmd.msg_text.act_val << ": ";
	cout << tMsgCmd.msg_text.act_unit << ": ";
	cout << tMsgCmd.msg_text.act_time << ": ";
	cout << tMsgCmd.msg_text.act_precs << ": ";
	cout << tMsgCmd.msg_text.act_min << ": ";
	cout << tMsgCmd.msg_text.act_max << ": ";
	cout << tMsgCmd.msg_text.act_stat << ": ";
	cout << tMsgCmd.msg_text.act_stat_time << endl;

	msgsnd.send_msg(tMsgCmd, sizMsg);
	   
	/* msgsnd.send_msg(tMsgCmd, sizMsg); */

	dp.cmdQueue.pop_front();
    }
    /* } */
    /* else */
    /* { */
    /* 	// send message for get ip, port, ssid, password. etc. */
    /* 	// 在此处发送一个结构，告诉本地层，需要获取保存在本地的服务器的ip 端口。 */
	
    /* } */


}


int socketConnect()
{
    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketTOServer == -1)
    {
	return -1;
    }

    struct sockaddr_in s_add; 

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    inet_pton(AF_INET, serverIP, &s_add.sin_addr);
    s_add.sin_port=htons(serverPort); 

    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
	printf("connect fail !\r\n");
	return -1;
    }

    return 0;
}

void *commToLocal(void *arg)
{
    //send message to local
    // sendmessageToLocal
}


// 返回的str字符串，需要dp.parese 解包之后，再判断是不是需要通讯层处理的。
int readLocalMessage(string &str)
{
    MsgData tmsg;
    /* memset(&tmsg.msg_text, 0, sizeof(tmsg.msg_text)); */
    /* if (!msgrecv_thrd->msg_is_empty()) { */
    /* 	tmsg = msgrecv_thrd->get_cur_msg(); */
    /* 	str = (char*)tmsg.msg_text; */
    /* 	return 0; */
    /* } */

    msg.read_message(message_t, &tmsg, 1);
    str = (char*)tmsg.msg_text;

    cout << str << endl;
    return 0;
}

void *localToServer(void *arg)
{
    MsgData tmsg;
    for ( ;  ;  )
    {
	string str;
//	cout << "44444read message" << endl;
	readLocalMessage(str);

//	sleep(10);
//	cout << " str = " << str << endl;

	/* if(need to Send to server) */
	/* { */

	if (str.length() > 0)
	{
	    if (connectModel == 0)
	    {
		socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
		if (socketTOServer == -1)
		{
//send message to local.
		}

		struct sockaddr_in s_add; 

		bzero(&s_add,sizeof(struct sockaddr_in));
		s_add.sin_family=AF_INET;
		inet_pton(AF_INET, serverIP, &s_add.sin_addr);
		s_add.sin_port=htons(serverPort); 

		if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
		{
		    printf("connect fail111 !\r\n");

		}

		if (write(socketTOServer, str.c_str(), strlen(str.c_str())) == -1)
		{
		    printf ("write error 4\n");

		}
		int recbytes;
		for ( ;  ;  )
		{
		    printf ("readind..............\n");
		    memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
		    if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
		    {
			printf("read data fail !\n");
			break;
		    }
		    /* printf("read ok\nREC:\n"); */

		    /* buffer[recbytes]='\0'; */
		    /* printf("%s\n",buffer); */

		    /* strcpy(buffer, text6); */
		    filterData(buffer);
		    break;
		}

		cout << "shorconnect" << endl;
		closeConnect();

	    }
	    else if (connectModel == 1)
	    {
//		alarm(0);
		cout << "socketToServer = " << socketTOServer << endl;
		if (socketTOServer == -1) // -1 meaning the socket is closed.
		{
		    socketConnect();
		}
		if (write(socketTOServer, str.c_str(), str.length()) == -1)
		{
		    printf ("write error 2\n");
		}
//		signal(SIGALRM, sendHeart);
//		alarm(10);
		//cout << "write ok" << endl;
	    }

	    /* } */
	    /* else */
	    /* { */
	    /* process by comm app */
	    /* } */

	}

    }
}

void *serverToLocal(void *arg)
{
    /* Msg_send msgsnd("tpt_snd_ctl_rcv"); */

    /* size_t sizMsg = sizeof(SrvCmd)+sizeof(long); */
    /* MsgCmd tMsgCmd; */


    /* char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"2\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhe\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"},{\"actType\":\"1\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]},{\"devID\":\"aa00ddeeffhh\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]}]}]}";  */

    /* cout << "parse data" << endl; */

    /* dp.parse(text6); */
    
    /* string str; */
    /* dp.packageMutipleCtrl(dp.cmdQueue, str); */
    
    /* string oldstr = "\n"; */
    /* trim(str,oldstr, 0); */
    /* oldstr = "\t"; */
    /* trim(str, oldstr, 0); */
    /* oldstr = " "; */
    /* trim(str, oldstr, 0); */
    /* oldstr = "\r"; */
    /* trim(str, oldstr, 0); */

    /* char temp[4]; */
    /* sprintf(temp, "%04d", str.length()); */
    /* //str = temp+str; */
    /* cout <<"str == " << str << endl; */



    
/*     int recbytes; */
/*     int sin_size; */

/*     char buffer[MAX_DATA_SEND_TO_SERVER]={0};    /\* 接受缓冲区 *\/ */

/*     //char heart[] = "{\"optCode\":\"heart\",\"devID\":\"34bca6010203\"}"; */
    
/*     int strlen = str.length(); */
/*     char heart[MAX_DATA_SEND_TO_SERVER] = {0}; */

/*     int aa = str.length(); */
/*     strncpy(heart, str.c_str(), aa); */
    
/*     heart[aa] = '\0'; */
/*     cout << "heart = " << heart << endl; */

/*     int ret; */
/*     unsigned short portnum=0x8888;  /\* 服务端使用的通信端口，可以更改，需和服务端相同 *\/ */
/* //    unsigned short portnum=14567; */

    for ( ;  ;  )
    {
	sleep(1);
	if (connectModel == 1)
	{
	    connectIsOK = 0; 

	    closeConnect();

	    printf ("has Data to send!\n");

	    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketTOServer == -1)
	    {
		//create socket failed.
		//send message to local.
	    }

	    struct sockaddr_in s_add; 

	    bzero(&s_add,sizeof(struct sockaddr_in));
	    s_add.sin_family=AF_INET;
	    inet_pton(AF_INET, serverIP, &s_add.sin_addr);
	    s_add.sin_port=htons(serverPort); 

	    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	    {
		printf("connect fail !\r\n");
		continue;
	    }

	    cout << "connecct OK" << endl;
	    // connect successful, set connectIsOK = 1
	    connectIsOK = 1;

	    //comment this code, do not send init at first.
	    /* if (write(socketTOServer, "init", 4) == -1) */
	    /* { */
	    /* 	printf ("write error\n"); */

	    /* } */

	    fd_set fdsr;
	    struct timeval tv;
	    int maxsock;
	    maxsock = socketTOServer;

	    int ret = -1;
	    while(1 == connectIsOK)
	    {
		FD_ZERO(&fdsr);

		FD_SET(socketTOServer, &fdsr);

		// timeout setting
		tv.tv_sec = 9;
		tv.tv_usec = 0;

		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);

		if (ret < 0) {
		    perror("select");
		    break;
		} else if (ret == 0) {
//		    printf("timeout\n");
		    /* continue; */
		}

		if (FD_ISSET(socketTOServer, &fdsr))
		{

		    ret = read(socketTOServer, buffer, MAX_DATA_SEND_TO_SERVER);

		    if (ret <= 0)
		    {        
			/* server close this socket connect. */
			printf("server close\n");

			closeConnect();
			connectIsOK = 0;
		    } 
		    else
		    {        
			/* receive data from server */
			if (ret < MAX_DATA_SEND_TO_SERVER)
			{
			    memset(&buffer[ret], '\0', 1);
			}
			printf("-----------%s------------\n", buffer);
			filterData(buffer);
		    }
		}
	    }
	}
	else
	{
	    return NULL;
	}
    }
}



void *sendHeart(void *arg)
{
    char heart[] = "{\"optCode\":\"heart\",\"devID\":\"34bca6010203\"}";

    while(1)
    {
	sleep(10);
	if (1 == connectIsOK && 1 == connectModel)
	{
	    cout << "heart" << endl;
	    try{
		if (write(socketTOServer, heart, sizeof(heart)) == -1)
		{
		    printf ("write error 3\n");
		    cout << "errno = " << errno << endl;
		}
	    }	    
	    catch(exception err)
	    {

	    }
	}
	else
	{
	    return NULL;
	}
    }
}




/**************************/
/* 与服务器连接的主要函数 */
/**************************/
void *connectServer(void *arg)
{
    Msg_send msgsnd("tpt_snd_ctl_rcv");

    size_t sizMsg = sizeof(SrvCmd)+sizeof(long);
    MsgCmd tMsgCmd;

    /* char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]},{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}"; */

    /* dp.parse(text6); */
    /* dp.parse(text6); */
    /* dp.parse(text6); */
    /* dp.parse(text6); */

    char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"2\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhe\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"},{\"actType\":\"1\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]},{\"devID\":\"34bca6010203\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]}]}]}"; 

    cout << "parse data" << endl;

    dp.parse(text6);
    
    string str;
    dp.packageMutipleCtrl(dp.cmdQueue, str);
    
    string oldstr = "\n";
    trim(str,oldstr, 0);
    oldstr = "\t";
    trim(str, oldstr, 0);
    oldstr = " ";
    trim(str, oldstr, 0);
    oldstr = "\r";
    trim(str, oldstr, 0);

    char temp[4];
    sprintf(temp, "%04d", str.length());
    //str = temp+str;
    cout <<"str == " << str << endl;


//    dp.parse(str.c_str());
    
    int recbytes;
    int sin_size;

    char buffer[MAX_DATA_SEND_TO_SERVER]={0};    /* 接受缓冲区 */

    //char heart[] = "{\"optCode\":\"heart\",\"devID\":\"34bca6010203\"}";
    
    int strlen = str.length();
    char heart[MAX_DATA_SEND_TO_SERVER] = {0};

    int aa = str.length();
    strncpy(heart, str.c_str(), aa);
    
    heart[aa] = '\0';
    cout << "heart = " << heart << endl;
//    strcpy(heart, str.c_str());

//    char *heart = const_cast<char *>(str.c_str());
    int ret;
    unsigned short portnum=0x8888;  /* 服务端使用的通信端口，可以更改，需和服务端相同 */
//    unsigned short portnum=14567;

    printf ("%s\n",*(char**)arg);


    for ( ;  ;  )
    {

	if (connectModel == -1)
	{
	    printf ("connect init\n");
//一次socket 连接到server, 通知server
	    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketTOServer == -1)
	    {
//send message to local.
	    }

	    struct sockaddr_in s_add; 

	    bzero(&s_add,sizeof(struct sockaddr_in));
	    s_add.sin_family=AF_INET;
	    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
	    s_add.sin_port=htons(portnum); 

	    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	    {
		printf("connect fail !\r\n");

	    }

	    if (write(socketTOServer, "init", 4) == -1)
	    {
		printf ("write error 4\n");

	    }
	    for ( ;  ;  )
	    {
		printf ("read\n");
		memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
		if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
		{
		    printf("read data fail !\n");
		}
		printf("read ok\nREC:\n");

		buffer[recbytes]='\0';
		printf("%s\n",buffer);
		break;
	    }

//	    connectModel = changeConnetModel(buffer);

	    if (dp.parse(buffer) == 0)
	    {
//need send to local app.
	    }


	    printf ("close connect\n");
	    connectModel = 1;
	    closeConnect();

	}
	if (connectModel == 0)//short socket connection
	{
	    if (hasDataToSend)
	    {
//send data to server
		printf ("has Data to send!\n");

		socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
		if (socketTOServer == -1)
		{
		    //send message to local.
		}

		struct sockaddr_in s_add; 

		bzero(&s_add,sizeof(struct sockaddr_in));
		s_add.sin_family=AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
		s_add.sin_port=htons(portnum); 

		if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
		{
		    printf("connect fail !\r\n");

		}

//need
/* printf ("%s\n",qbuf.mtext); */
/* if (write(socketTOServer, qbuf.mtext, sizeof(qbuf.mtext)) == -1) */
/* { */
/*     printf ("write error\n"); */

/* } */
		for ( ;  ;  )
		{
		    memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
		    if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
		    {
			printf("read data fail !\n");
		    }
		    printf("read ok\nREC:\n");

		    buffer[recbytes]='\0';
		    printf("%s\n",buffer);
		    dp.parse(buffer);
		    sendmessageToLocal();
		    break;
		}


		printf ("close this short connect\n");
		closeConnect();
//		connectModel = changeConnetModel(buffer);

		hasDataToSend = false;

	    }
	    else
	    {
		//do nothing.
	    }
	}

	if (connectModel == 1)// long socket connection
	{
	    //send data to server, Although local app not send data, the comm app need send "heart" to server.


	    int connectIsOK = 0; 

	    closeConnect();

	    printf ("has Data to send!\n");

	    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketTOServer == -1)
	    {
		//create socket failed.
		//send message to local.
	    }


	    struct sockaddr_in s_add; 

	    bzero(&s_add,sizeof(struct sockaddr_in));
	    s_add.sin_family=AF_INET;
	    inet_pton(AF_INET, "127.0.0.1", &s_add.sin_addr);
	    s_add.sin_port=htons(portnum); 

	    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	    {
		printf("connect fail !\r\n");
		continue;
	    }

	    // connect successful, set connectIsOK = 1
	    connectIsOK = 1;

	    //comment this code, do not send init at first.
	    /* if (write(socketTOServer, "init", 4) == -1) */
	    /* { */
	    /* 	printf ("write error\n"); */

	    /* } */


	    fd_set fdsr;
	    struct timeval tv;
	    int maxsock;
	    maxsock = socketTOServer;

	    while(1 == connectIsOK)
	    {
		FD_ZERO(&fdsr);

		FD_SET(socketTOServer, &fdsr);

		// timeout setting
		tv.tv_sec = 9;
		tv.tv_usec = 0;

		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);

		if (ret < 0) {
		    perror("select");
		    break;
		} else if (ret == 0) {
//		    printf("timeout\n");
		    /* continue; */
		}

		if (FD_ISSET(socketTOServer, &fdsr))
		{

		    ret = read(socketTOServer, buffer, MAX_DATA_SEND_TO_SERVER);

		    if (ret <= 0)
		    {        
			/* server close this socket connect. */
			printf("server close\n");

			closeConnect();
			connectIsOK = 0;

			/* connectModel = 0; */
			connectModel = 1;
		    } 
		    else
		    {        
			/* receive data from server */
			if (ret < MAX_DATA_SEND_TO_SERVER)
			{
			    memset(&buffer[ret], '\0', 1);
			}
			printf("-----------%s------------\n", buffer);
			dp.parse(buffer);
			sendmessageToLocal();
		    }
		}


		if (hasDataToSend)
		{

		    //send data to server
//此处代码在7-10日的时候注释掉。原因是修改发送数据给本地层的代码时，qbuf提示没有定义。  此处用到qbuf是因为可以直接把从本地层接收到的数据直接发送给服务器。
		    /* printf ("%s\n",qbuf.mtext); */
		    /* if (write(socketTOServer, qbuf.mtext, sizeof(qbuf.mtext)) == -1) */
		    /* { */
		    /* 	printf ("write error\n"); */

		    /* } */

		    /* 以后可以试试不用下面的代码，而使用select来查询是否有服务器的数据。 */
		    for ( ;  ;  )
		    {
			memset(buffer, 0, MAX_DATA_SEND_TO_SERVER);
			if(-1 == (recbytes = read(socketTOServer,buffer,MAX_DATA_SEND_TO_SERVER)))
			{
			    printf("read data fail !\n");
			}
			printf("read ok\nREC:\n");

			buffer[recbytes]='\0';
			printf("%s\n",buffer);
//			connectModel = changeConnetModel(buffer);
			break;
		    }

		    hasDataToSend = false;
		}

	    }
	}
    }
}

/* void localData(int i) */
/* { */
/*     msg.read_message(message_t, (struct mymsgbuf *) &qbuf, 2); */
/*     hasDataToSend = true; */
/* } */

/* void clear(int i) */
/* { */
/*     printf ("clear\n"); */
/*     msg.remove_queue(message_t); */
/*     exit(0); */
/* } */


/* void *getlocalID(void *arg) */
/* { */
/*     getOtherID otherID; */
    
/*     const char * local = "local"; */
/*     for (; ; ) */
/*     { */
/* 	localID = otherID.getID(local); */
/* //	printf ("%d\n",localID); */
/* 	sleep(1); */
/*     } */
/* } */

int shortConnect()
{
    printf ("connect init\n");
//一次socket 连接到server, 通知server
    socketTOServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketTOServer == -1)
    {
//send message to local.
    }

    struct sockaddr_in s_add; 

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    inet_pton(AF_INET, serverIP, &s_add.sin_addr);
    s_add.sin_port=htons(serverPort); 

    if(-1 == connect(socketTOServer,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
	printf("connect fail !\r\n");

    }

    if (write(socketTOServer, buffer, strlen(buffer)) == -1)
    {
	printf ("write error 5\n");
    }
}


/* get some info from local app */

int main(int argc,char ** argv)
{


    // 测试packageMutipleCtrl
    /* char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"2\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhe\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"},{\"actType\":\"1\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]},{\"devID\":\"34bca6010203\",\"numOfCont\":\"2\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]},{\"contID\":\"1\",\"contType\":\"500\",\"numOfAct\":\"2\",\"actArray\":[{\"actType\":\"1\",\"actValue\":\"0\"},{\"actType\":\"2\",\"actValue\":\"0\"}]}]}]}"; */


/*     char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}"; */



/*     cout << "parse data" << endl; */

/*     dp.parse(text6); */
    
/*     string str; */
/*     dp.packageMutipleCtrl(dp.cmdQueue, str); */
/*     /\* string oldstr = "\n"; *\/ */
/*     /\* trim(str,oldstr, 0); *\/ */
/*     /\* oldstr = "\t"; *\/ */
/*     /\* trim(str, oldstr, 0); *\/ */
/*     cout <<"str == " << str << endl; */

/* //    dp.parse(str.c_str()); */
/*     return 0; */
    // 测试packageMutipleCtrl结束



    //====================================================================================================
    // 测试packageMutipleSensor


/* {"sysID":"1","seqOfIns":"1","numOfDev":"1","optCode":"0030","timeout":"1234756609589", */
/*     "devArray":[{"devID":"34bca6010203","numOfCont
       ":"1", */
/*         "senArray":[{"senID":"0","senType":"0005","numOfParam":"1", */
/* 			"paramArray":[{"paramType":"0055","paramValue":"220"},{"paramType":"0060","paramValue":"10"}]}]}]} */

    /* deque<MeteringData> dataQue; */
    /* MeteringData meterData; */

    /* strcpy(meterData.sys_id, "1"); */
    /* meterData.dev_no = 1; */
    /* strcpy(meterData.op_code, "0040"); */
    
    /* strcpy(meterData.dev_id, "aabbccddeeff"); */
    /* meterData.sensor_no = 2; */

    /* strcpy(meterData.sensor_id, "1"); */
    /* strcpy(meterData.sensor_typ, "1type"); */
    /* meterData.param_no = 2; */
    
    /* strcpy(meterData.param_typ, "partyp"); */
    /* strcpy(meterData.param_Val, "parval"); */
    
    /* dataQue.push_back(meterData); */

    /* strcpy(meterData.sys_id, "1"); */
    /* meterData.dev_no = 1; */
    /* strcpy(meterData.op_code, "0040"); */
    
    /* strcpy(meterData.dev_id, "aabbccddeeff"); */
    /* meterData.sensor_no = 2; */

    /* strcpy(meterData.sensor_id, "1"); */
    /* strcpy(meterData.sensor_typ, "1type"); */
    /* meterData.param_no = 2; */
    
    /* strcpy(meterData.param_typ, "partyp2"); */
    /* strcpy(meterData.param_Val, "parval2"); */

    /* dataQue.push_back(meterData); */

    /* strcpy(meterData.sys_id, "1"); */
    /* meterData.dev_no = 1; */
    /* strcpy(meterData.op_code, "0040"); */
    
    /* strcpy(meterData.dev_id, "aabbccddeeff"); */
    /* meterData.sensor_no = 2; */

    /* strcpy(meterData.sensor_id, "2"); */
    /* strcpy(meterData.sensor_typ, "2type"); */
    /* meterData.param_no = 2; */
    
    /* strcpy(meterData.param_typ, "partyp3"); */
    /* strcpy(meterData.param_Val, "parval3"); */
    
    /* dataQue.push_back(meterData); */

    /* strcpy(meterData.sys_id, "1"); */
    /* meterData.dev_no = 1; */
    /* strcpy(meterData.op_code, "0040"); */
    
    /* strcpy(meterData.dev_id, "aabbccddeeff"); */
    /* meterData.sensor_no = 2; */

    /* strcpy(meterData.sensor_id, "2"); */
    /* strcpy(meterData.sensor_typ, "2type"); */
    /* meterData.param_no = 2; */
    
    /* strcpy(meterData.param_typ, "partyp4"); */
    /* strcpy(meterData.param_Val, "parval4"); */

    /* dataQue.push_back(meterData); */

    /* /\* while(dataQue.size()) *\/ */
    /* /\* { *\/ */
    /* /\* 	cout << dataQue.front().param_typ << endl; *\/ */
    /* /\* 	dataQue.pop_front(); *\/ */
    /* /\* } *\/ */
    /* string str; */
    /* dp.packageMutipleSensor(dataQue, str); */

    /* cout << "str = " << str << endl; */

    /* return 0; */

    // 测试packageMutipleSensor 结束
    // ====================================================================================================





    /************************************************************************
    /* char text6[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\"}"; */



    /* string optCode = dp.checkOptCode(text6); */
    /* cout << optCode << endl; */

    /* return 0; */



    // 此处创建消息队列，用来读取本地层发送的信息
    message_t = msg.create_queue();
    cout << "id = " << message_t << endl;


    /* ============================== */
    // 创建与本地层交互的消息队列   会启动一个线程循环读取此消息队列，并把消息暂存在msgrecv_thrd对象中。
    /* msgrecv_thrd = new Msg_recv("tpt_snd_ctl_rcv_data"); */
    /* atexit(destroy_msgrecv_thrd); */
    /* /\* cout << "Msg_recv created" << endl; *\/ */
    /* msgrecv_thrd->start(); */
    /* cout << "Msg_recv started" << endl; */

    /* int flag = fcntl(0, F_GETFL, 0); */
    /* flag |= O_NONBLOCK; */
    /* if (fcntl(0, F_SETFL, flag) < 0) {      /\* fgetc no-block now *\/ */
    /* 	cerr << "fcntl() error: Set stdin to non-block fails." << strerror(errno) << endl; */
    /* } */

    int temp; 
    memset(&thread, 0, sizeof(thread));

    /* 发送心跳 */
    /* if((temp = pthread_create(&thread[0], NULL, sendHeart, NULL)) != 0) */
    /* 	printf("create thread for send heart failed !\n"); */
    /* else */
    /* 	printf("create thread for send heart successed !\n"); */

    // 读取消息队列   读取msgrecv_thrd中保存的消息。
    if((temp = pthread_create(&thread[1], NULL, localToServer, NULL)) != 0)
	printf("create thread for localToServer failed !\n");
    else
	printf("create thread for localToServer successed !\n");


    deviceModel = AP;

//    apmodel.start();
    apmodel.run();
    cout << "ap end: " << endl;

    cout << "serverIP = " << serverIP << endl;
    cout << "serverPORT = " << serverPort << endl;

    struct scanresults *results=NULL;
    tosta("ra0");
    sta_scan();
    sleep(3);

    results=sta_scanresults();
    for(int i=0;i<results->count;i++)
    {
	//printf("\nNo.%d\n",i);
	printf("%-4d%-33s%02x:%02x:%02x:%02x:%02x:%02x \n",results->result[i]->channel,results->result[i]->ssid,
	       results->result[i]->bssid[0],results->result[i]->bssid[1],results->result[i]->bssid[2],results->result[i]->bssid[3],
	       results->result[i]->bssid[4],results->result[i]->bssid[5]);
    }
    sta_connect("startech-218","startech1");
    int result;
    for(int j = 0; j < 15; j ++)
    {
	result=sta_linkstatu();
	printf("linkstau is %d\n",result);
	sleep(1);
    }

    cout << "udhcpc...." << endl;
    system("udhcpc -i ra0 -s /sbin/udhcpc.sh -p /var/run/udhcp");
    cout << "sleep(4)....." << endl;
				
    sleep(4);
	
    cout << "start get ip" << endl;
    string ip = utility.getIP("ra0");
    if(ip != "")
    {
	cout << "kill udhcpc..." << endl;
	system("killall udhcpc");
	cout << "utility.getIP()= " << ip << endl;
    }
    /* toap(); */
    /* cout << "sleep(10)" << endl; */
    /* sleep(10); */
    /* startSocketServer(); */
    
    /* signal(SIGUSR1, localData); */
    /* signal(SIGINT, clear); */


    while(1)
    {
	sleep(10);

//注释此些行，不用这些代码来发数据给local，而是等server发来数据之后，再发给local。
// 	if (localID > 0)
// 	{
// 	    char cmd[256];
// 	    memset(cmd, 0, 256);
// 	    printf ("please input a msessage(send to local):");
// 	    fgets(cmd, 255, stdin);
// 	    msg.send_message(message_t, (struct mymsgbuf *) &qbuf, 1, cmd);
// 	    kill(localID, SIGUSR1);
// //	    sleep(3);
// 	}
    }
}

