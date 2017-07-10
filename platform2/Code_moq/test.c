/*
 * =====================================================================================
 *
 *    Filename:  test.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  28 Jun 2017  14:19:53
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   28 Jun 2017  14:19:53 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for sleep
#include <pthread.h> //for pthread*

/*------------Local Variables----------- */
//定义一些本地变量



/*------Local Structures and Typedefs ---*/
//要使用的一些数据结构



/*-----------Extern Variables -----------*/
//使用到的一些外部变量



/*-------------Definitions---------------*/
//一些#defines及具体的函数实现


#include <stdio.h>
//#include </usr/local/include/mosquitto.h>
#include <mosquitto.h>


struct mosquitto *mosq = NULL;




void *startech_write(void *arg){
  for(;;){
  printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n" );
  int rc = mosquitto_publish(mosq, NULL, "isocketsTest/device/aaaaaa", 10, "uuuuuuuuuu", 0, false);
  printf("publish rc = %d\n", rc);
  sleep(20);
  }
}

/* void *startech_read(void *arg){ */
  
/* } */

void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		printf("%s %s\n", (char*)(message->topic), (char*)(message->payload));
	}else{
		printf("%s (null)\n", message->topic);
	}
	fflush(stdout);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
  //	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
    //		mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
    int rc = mosquitto_subscribe(mosq, NULL, "isocketsTest/Server/#", 0);
    printf("subscribe rc = %d\n", rc);

    printf("connect successful\n");
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void my_disconnect_callback(struct mosquitto *mosq, void *userdata, int result)
{
  //	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */

    //		mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
    printf("disconnect\n");
	}else{
		fprintf(stderr, "Connect failed\n");
	}
  
}
void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);
}


int main(int argc, char *argv[])
{
  //	int i;
  //	char *host = "85.119.83.194";
  //  char *host = "37.187.106.16";
  char *host = "test.mosquitto.org";
	int port = 8883;
	int keepalive = 60;
	bool clean_session = true;


	mosquitto_lib_init();
	mosq = mosquitto_new("isocketssssss", clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

  mosquitto_tls_set(mosq, "mosquitto.org.crt", NULL, NULL, NULL, NULL);
	mosquitto_log_callback_set(mosq, my_log_callback);
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
  mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);

	if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}






  int temp;
  pthread_t pt_startech_write;
  if ((temp = pthread_create(&pt_startech_write, NULL, startech_write, NULL)) != 0) {
    printf("create thread for startech_write failed !");
  }
  else{
    printf("create thread for startech_write successed !");
  }

  mosquitto_loop_forever(mosq, -1, 1);

  /* pthread_t pt_startech_read; */
  /* if ((temp = pthread_create(&pt_startech_read, NULL, startech_read, NULL)) != 0) { */
  /*   debug_msg("create thread for startech_read failed !"); */
  /* } */
  /* else{ */
  /*   debug_msg("create thread for strtech_read successed !"); */
  /* } */


  while(1){
    sleep(1);
  }

	return 0;
}
