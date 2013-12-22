/* 
 *     Created:  24  7月 2013 16:36:33
 *     Author:   weishijian@weishijian-MacUbuntu
 Description:  
*/

//#include <config.h>
#include "APModel.h"

#include "includes.h"
#include "common.h"
#include "wpa_supplicant_i.h"
#include "customopt.h"
#include "wifitypes.h"
#include "apoperation.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "global.h"
#include "phoneDataParse.h"
#include <deque>
#include "utility.h"
#include <iostream>

using namespace std;


#define MYPORT 0x8888    // the port users will be connecting to
#define BACKLOG 5     // how many pending connections queue will hold
#define BUF_SIZE 1024
int fd_A[BACKLOG];    // accepted connection fd
int conn_amount;    // current connection amount

bool pair_OK = false;

void showclient()
{
    int i;

    printf("client amount: %d\n", conn_amount);

    for (i = 0; i < BACKLOG; i++) {
        printf("[%d]:%d  ", i, fd_A[i]);
    }

    printf("\n\n");
}


extern enum deviceModel deviceModel;

extern char serverIP[15];
extern int serverPort;
extern char ssid[128];
extern char password[128];



PairInfo pairInfo;

string str;


APModel::APModel() : Thread()
{
}


APModel::~APModel()
{
}


void *APModel::run()
{
    toap();
    sleep(3);


    int sock_fd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in server_addr;    // server address information
    struct sockaddr_in client_addr; // connector's address information
    socklen_t sin_size;
    int yes = 1;
    char buf[BUF_SIZE];
    int ret;
    int i;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }


    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    {
        perror("setsockopt");
        exit(1);
    }


    server_addr.sin_family = AF_INET;         // host byte order

    server_addr.sin_port = htons(MYPORT);     // short, network byte order

    server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP

    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));


    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(1);
    }


    if (listen(sock_fd, BACKLOG) == -1) 
    {
        perror("listen");
        exit(1);
    }

    printf("listen port %d\n", MYPORT);

    fd_set fdsr;
    int maxsock;
    struct timeval tv;

    conn_amount = 0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;
    while (!pair_OK)
    {
        // initialize file descriptor set
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);

        // timeout setting
        tv.tv_sec = 30;
        tv.tv_usec = 0;


        // add active connection to fd set
        for (i = 0; i < BACKLOG; i++) 
	{
            if (fd_A[i] != 0) 
	    {
                FD_SET(fd_A[i], &fdsr);
            }
        }


        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);

        if (ret < 0) 
	{
            perror("select");
            break;
        } 
	else if (ret == 0) 
	{
            printf("timeout\n");
            continue;
        }

        // check every fd in the set

        for (i = 0; i < conn_amount; i++) 
	{
            if (FD_ISSET(fd_A[i], &fdsr)) 
	    {
                ret = recv(fd_A[i], buf, sizeof(buf), 0);
                if (ret <= 0)    // client close
		{     
		    conn_amount --;
                    printf("client[%d] close\n", i);
                    close(fd_A[i]);
                    FD_CLR(fd_A[i], &fdsr);
                    fd_A[i] = 0;
                }
		else
		{        // receive data

                    if (ret < BUF_SIZE)
		    {
                        memset(&buf[ret], '\0', 1);
		    }
                    printf("client[%d] send:%s\n", i, buf);

		    filterData(buf);
		    
		    send(fd_A[i], str.c_str(), strlen(str.c_str()), 0);
		    //send(fd_A[i], buf, sizeof(buf), 0);
                }
            }
        }


        // check whether a new connection comes
        if (FD_ISSET(sock_fd, &fdsr)) 
	{

            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);

            if (new_fd <= 0) 
	    {

                perror("accept");

                continue;

            }



            // add to fd queue

            if (conn_amount < BACKLOG) 
	    {

                /* fd_A[conn_amount++] = new_fd; */
		for (i = 0; i < BACKLOG; i++)
		{
		    if (fd_A[i] == 0)
		    {
			fd_A[i] = new_fd;
			break;
		    }
		}
		conn_amount ++;
                printf("new connection client[%d] %s:%d\n", conn_amount,
		       inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                if (new_fd > maxsock)
		{
                    maxsock = new_fd;
		}
            }
            else 
	    {
                printf("max connections arrive, exit\n");
                send(new_fd, "bye", 4, 0);
                close(new_fd);
                break;
            }
        }
        showclient();
    }

    // close other connections
    for (i = 0; i < BACKLOG; i++) 
    {
        if (fd_A[i] != 0) 
	{
            close(fd_A[i]);
        }
    }

    cout << "close socket" << endl;
    cout << "kill udhcpc..." << endl;
    system("killall udhcpc");
    close(sock_fd);
}


void APModel::filterData(const char *buffer)
{
    cout << "filterData" << endl;
    phoneDataParse phonedp;
    
    char *optcode = phonedp.checkOptCode(buffer);
    char temp[10];
    bzero(temp, sizeof(temp));
    strncpy(temp, optcode, 4);

    if (strcmp(temp, "0010") == 0)
    {
	getWIFIList(str);
    }
    else if (strcmp(temp, "0020") == 0)
    {
	// pair;
	if (phonedp.parsePairData(buffer, pairInfo) == 0)
	{
	    // set up device
	    /* cout << "pairInfo.ssid" << pairInfo.ssid << endl; */
	    /* cout << "pairInfo.mask = " << pairInfo.mask << endl; */
	    /* cout << "pairInfo.dns2 = " << pairInfo.dns2 << endl; */
	    /* cout << "pairInfo.dns1 = " << pairInfo.dns1 << endl; */
	    /* cout << "pairInfo.sys_id = " << pairInfo.sys_id << endl; */
	    /* cout << "pairInfo.timeOut = " << pairInfo.timeOut << endl; */
	    /* cout << "pairInfo.op_code = " << pairInfo.op_code << endl; */
	    /* cout << "pairInfo.ip_addr = " << pairInfo.ip_addr << endl; */
	    /* cout << "pairInfo.password = " << pairInfo.password << endl; */
  	 

	    cout << "str = " << str << endl;
	    cout << "connecting ...." << endl;

	    ap_connect("apcli0", pairInfo.ssid, pairInfo.password);

	    cout << "sleep(5)....." << endl;
	    sleep(5);

	    cout << "check link status...." << endl;
	    int i = 0;
	    int k = 0;
	    for(k = 0; k < 15; k ++)
	    {

		i=query_ap_connectstatu("apcli0");
		printf("linkstau is %d\n",i);
		
		sleep(1);
	    }

	    if(i < 3 && i > 0)
	    {
		phonedp.packagePairResult(str, "0210");
		return;
	    }
	    if(i < 7 && i >= 3)
	    {
		phonedp.packagePairResult(str, "0210");
		return;
	    }
	    if(i == 7)
	    {

		if(pairInfo.dhcp_status == 1)
		{
		    cout << "udhcpc...." << endl;
		    system("udhcpc -i apcli0 -s /sbin/udhcpc.sh -p /var/run/udhcp");
		    cout << "sleep(4)....." << endl;
				
		    sleep(4);
		    utility utility;
		    cout << "start get ip" << endl;
		    string ip = utility.getIP("apcli0");
		    if(ip != "")
		    {
			phonedp.packagePairResult(str, "9999");

			cout << "utility.getIP()= " << ip << endl;
		    }
				
		}
		pair_OK = true;
		memset(serverIP, 0, 15);
		memset(ssid, 0, 128);
		memset(password, 0, 128);


		strcpy(serverIP, pairInfo.server_ip);
		strcpy(ssid, pairInfo.ssid);
		strcpy(password, pairInfo.password);
	
		serverPort = pairInfo.server_port;
		return;
	    }
	}

    }
}


int APModel::setupDevice(string &str)
{
    
}


int APModel::getWIFIList(string &str)
{
    deque<WiFiInfo> wifiList;
    WiFiInfo wifiInfo;
    int i = 0;
 
    struct scanresults *results = NULL;
    ap_scan("ra0");

    sleep(4);

    results=ap_scanresult("ra0");


    if (results == NULL)
    {
    	printf("results is NULL\n");
    	return -1;
    }
    else
    {
    	for(i=0;i<results->count;i++)
    	{

    	    strcpy(wifiInfo.ssid, (char*)results->result[i]->ssid);

    	    wifiInfo.rssi = results->result[i]->siganl;

    	    strcpy(wifiInfo.encryption, results->result[i]->ap_auth_mode);

    	    wifiList.push_back(wifiInfo);
	

    	    /*printf("\nNo.%d\n",i);
    	      printf("%-4d%-33s%02x:%02x:%02x:%02x:%02x:%02x   %s/%s        %d\n",results->result[i]->channel,results->result[i]->ssid,
    	      results->result[i]->bssid[0],results->result[i]->bssid[1],results->result[i]->bssid[2],results->result[i]->bssid[3],
    	      results->result[i]->bssid[4],results->result[i]->bssid[5],results->result[i]->ap_cipher,results->result[i]->ap_auth_mode,results->result[i]->siganl); */
    	}
    }

    phoneDataParse phonedp;
    phonedp.packageWiFiInfo(wifiList, str);

    cout << "str = " << str << endl;
    return 0;
}
