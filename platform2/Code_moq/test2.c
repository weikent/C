/*
 * =====================================================================================
 *
 *    Filename:  test2.c
 *
 * Description:  
 *
 *
 *
 *     Version:  0.1
 *     Created:  10 Jul 2017  09:02:51
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:
 * ======================================================================================
 * @0.1   weishijian   10 Jul 2017  09:02:51 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/
//包含的头文件


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */
//定义一些本地变量



/*------Local Structures and Typedefs ---*/
//要使用的一些数据结构



/*-----------Extern Variables -----------*/
//使用到的一些外部变量



/*-------------Definitions---------------*/
//一些#defines及具体的函数实现


#include <stdio.h>

#include <mosquitto.h>

void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
    printf("LOG: %s\n", str);
}

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
    //		mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
    printf("connect successful");
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

int main(int argc, char *argv[])
{

    struct mosquitto *mosq = NULL;
    int rc;

    /* printf("Calling connect before lib init, this should fail.\n"); */
    /* mosq = mosquitto_new(NULL, true, NULL); */
    /* mosquitto_log_callback_set(mosq, my_log_callback); */
    /* mosquitto_connect_callback_set(mosq, my_connect_callback); */
    /* mosquitto_tls_set(mosq, "mosquitto.org.crt", NULL, NULL, NULL, NULL); */
    /* rc = mosquitto_connect(mosq, "test.mosquitto.org", 8883, 60); */
    /* printf("connect returned %d\n", rc); */
    /* mosquitto_destroy(mosq); */



    mosquitto_lib_init();

    printf("Calling connect after lib init, this should be fine.\n");
    mosq = mosquitto_new(NULL, true, NULL);
    mosquitto_log_callback_set(mosq, my_log_callback);
    mosquitto_tls_set(mosq, "mosquitto.org.crt", NULL, NULL, NULL, NULL);
    rc = mosquitto_connect(mosq, "test.mosquitto.org", 8883, 60);
    printf("connect returned %d\n", rc);
    while(1){
      
    }
    mosquitto_destroy(mosq);


    mosquitto_lib_cleanup();


    /* printf("Calling connect after lib cleanup, this should fail.\n"); */
    /* mosq = mosquitto_new(NULL, true, NULL); */
    /* mosquitto_log_callback_set(mosq, my_log_callback); */
    /* mosquitto_tls_set(mosq, "mosquitto.org.crt", NULL, NULL, NULL, NULL); */
    /* rc = mosquitto_connect(mosq, "test.mosquitto.org", 8883, 60); */
    /* printf("connect returned %d\n", rc); */
    /* mosquitto_destroy(mosq); */

    return 0;
}
