/*
 * =====================================================================================
 *
 *       Filename:  message.h
 *
 *    Description:  Header file to descript struct message
 *
 *        Version:  1.0
 *        Created:  06/26/2013 04:26:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david.zhao
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef __MESSAGE_H_
#define __MESSAGE_H_
#include <unistd.h>
#include <time.h>

const int TEXT_SIZE = 1024;

struct SrvCmd {
    // Layer 1
    char sys_id[8];
    size_t ins_seq_no;
    size_t dev_no;
    char op_code[8];
    // Layer 2
    char dev_id[16];
    size_t ctrl_no;
    // Layer 3
    char ctrl_id[8];
    char ctrl_typ[8];
    size_t act_no;
    // Layer 4
    char act_typ[8];
    char act_val[8];
    char act_unit[8];
    time_t act_time;
    float act_precs;
    float act_min;
    float act_max;
    char act_stat[8];
    time_t act_stat_time;
};

struct MeteringData {
    // Layer 1
    char sys_id[8];
    size_t dev_no;
    char op_code[8];
    // Layer 2
    char dev_id[16];
    size_t sensor_no;
    // Layer 3
    char sensor_id[8];
    char sensor_typ[8];
    size_t param_no;
    // Layer 4
    char param_typ[8];
    char param_Val[8];
    char param_val_typ[8];
    char param_unit[8];
    time_t param_time;
    float param_precs;
    float param_min;
    float param_max;
};

// struct MsgCmd used to thrans message from Server to Local controller
struct MsgCmd {
    long msg_type;
    SrvCmd msg_text;
};

// struct MsgData used to thrans message from Local controller to Server 
struct MsgData {
    long msg_type;
    char msg_text[TEXT_SIZE];
};

#endif //__MESSAGE_H_

