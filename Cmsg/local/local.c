/* 
 *     Created:  22 May 2013 10:51:03
 *     Author:   weishijian@ubuntu
 Description:  
*/



#include "msgqueue.h"
#include "global/global.h"
#include "getOtherID.h"

#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
//#include <cstdio>
//#include <csignal>
#include <stdio.h>
#include <signal.h>





//msgqueue msg;
int message_t;
struct mymsgbuf qbuf;
pthread_t thread[1]; 
int commID;


void commData(int i)
{
//    printf ("%d\n",i);
    read_message(message_t, (struct mymsgbuf *) &qbuf, 1);
    printf ("Type: %ld Text: %s \n", qbuf.mtype, qbuf.mtext);

}

void exitApp(int i)
{
    printf ("exitApp\n");
    remove_queue(message_t);
    exit(0);
}


void *getcommID(void *arg)
{
    
    const char * comm = "comm";
    for (; ; )
    {
	commID = getID(comm);
//	printf ("commid = %d\n",commID);
	sleep(1);
    }
}


int main(int argc, char *argv[])
{

    int temp; 
    memset(&thread, 0, sizeof(thread));

    if((temp = pthread_create(&thread[0], NULL, getcommID, NULL)) != 0)
	printf("线程创建失败!\n"); 
    else 
	printf("线程被创建\n"); 


    signal(SIGUSR1, commData);
    signal(SIGINT, exitApp);


    message_t = create_queue();
    printf ("message_t = %d\n",message_t);
    while(1)
    {
	if (commID > 0)
	{
	    char cmd[256];
	    memset(cmd, 0, 256);
	    printf ("please input a msessage(send to comm):");
	    fgets(cmd, 255, stdin);
	    send_message(message_t, (struct mymsgbuf *) &qbuf, 2, cmd);
	    kill(commID, SIGUSR1);
//	    sleep(3);
	}
    }
    return 0;
}
