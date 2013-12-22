/*
 * =====================================================================================
 *
 *       Filename:  test_msgsnd.cpp
 *
 *    Description:  Test to use class Msg_send
 *
 *        Version:  1.0
 *        Created:  06/26/2013 06:19:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david.zhao
 *        Company:  Beijing Ding Qing Soft Co. Ltd.
 *
 * =====================================================================================
 */
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "msgsnd_data.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

const int BUF_SIZE = 64;

int main() {
//    Msg_send msgsnd("tpt_snd_ctl_rcv");

    // MsgCmd tmsg;
    // string tstr;
    // char buf[BUF_SIZE];

    // tmsg.msg_type = 2;
    // memset(&tmsg.msg_text, 0, TEXT_SIZE);
    // while (cin >> tstr) {
    //// while (fgets(buf, BUF_SIZE, stdin)) {
    ////     tmsg.msg_type += 0x10;
    ////     // memcpy(&tmsg.msg_text, tstr.c_str(), tstr.size());
    ////     memcpy(&tmsg.msg_text, buf, BUF_SIZE);
    ////     // cout << "tmsg.msg_text: " << tmsg.msg_text << endl;
    ////     msgsnd.send_msg(tmsg, BUF_SIZE);
    ////     memset(&tmsg.msg_text, 0, TEXT_SIZE);
    ////     // usleep(1000000);
    //// }

    // Test binary format MsgCmd
    // tmsg.msg_type = 0x10;
    // int tmp = 0xfedcba;
    // cout << "Size of tmp: " << sizeof(tmp) << endl;
    // for (int j=0; j<64; ++j) {
    //     memcpy(buf, &j, 1);
    // }
    // memcpy(&tmsg.msg_text, &tmp, sizeof(tmp));
    // tmsg.msg_text[0] = 0xfedcba;
    // msgsnd.send_msg(tmsg, TEXT_SIZE+4);
    // cout << tmsg.msg_text[0] << endl;

    // size_t sizMsg = sizeof(SrvCmd)+3*sizeof(CntrlAct)+sizeof(long);


    Msg_send msgsnd("tpt_snd_ctl_rcv_data");
    size_t sizMsg = sizeof(MsgData)+sizeof(long);
    MsgData tmsgData;
    tmsgData.msg_type = sizMsg;
//    tmsgData.msg_text = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhh\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}";

    /* memmove(tmsgData.msg_text, "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhh\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}", sizeof("{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0050\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"aa00ddeeffhh\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}")); */



    memmove(tmsgData.msg_text, "666", sizeof("666"));

    msgsnd.send_msg(tmsgData, sizMsg);
//    send(msgsnd);

    /* cout << "-------------------" << endl; */

    /* size_t sizMsg = sizeof(SrvCmd)+sizeof(long); */
    /* MsgCmd tMsgCmd; */
    /* tMsgCmd.msg_type = sizMsg; */
    /* memmove(tMsgCmd.msg_text.sys_id, "666", sizeof("666")); */
    /* tMsgCmd.msg_text.ins_seq_no = 2000; */
    /* tMsgCmd.msg_text.dev_no = 2; */
    /* memmove(tMsgCmd.msg_text.op_code, "wr_ee", sizeof("wr_ee")); */
    /* memmove(tMsgCmd.msg_text.dev_id, "34bca6012345", sizeof("34bca6012345")); */
    /* tMsgCmd.msg_text.ctrl_no = 6; */
    /* memmove(tMsgCmd.msg_text.ctrl_id, "1a", sizeof("1a")); */
    /* memmove(tMsgCmd.msg_text.ctrl_typ, "500", sizeof("500")); */
    /* tMsgCmd.msg_text.act_no = 3; */
    /* memmove(tMsgCmd.msg_text.act_typ, "relay", sizeof("relay")); */
    /* memmove(tMsgCmd.msg_text.act_val, "1", sizeof("1")); */
    /* memmove(tMsgCmd.msg_text.act_unit, "mV", sizeof("mV")); */
    /* tMsgCmd.msg_text.act_time = 01234567; */
    /* tMsgCmd.msg_text.act_precs = 0.005; */
    /* tMsgCmd.msg_text.act_min = 0.000001; */
    /* tMsgCmd.msg_text.act_max= 220.999; */
    /* memmove(tMsgCmd.msg_text.act_stat, "RelayOn", sizeof("RelayOn")); */
    /* tMsgCmd.msg_text.act_stat_time =76543210; */
    /* msgsnd.send_msg(tMsgCmd, sizMsg); */

    /* cout << tMsgCmd.msg_type << endl; */
    /* cout << tMsgCmd.msg_text.sys_id << ": "; */
    /* cout << tMsgCmd.msg_text.ins_seq_no << ": "; */
    /* cout << tMsgCmd.msg_text.dev_no << ": "; */
    /* cout << tMsgCmd.msg_text.op_code << ": "; */
    /* cout << tMsgCmd.msg_text.dev_id << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_no << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_id << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_typ << ": "; */
    /* cout << tMsgCmd.msg_text.act_no << ": "; */
    /* cout << tMsgCmd.msg_text.act_typ << ": "; */
    /* cout << tMsgCmd.msg_text.act_val << ": "; */
    /* cout << tMsgCmd.msg_text.act_unit << ": "; */
    /* cout << tMsgCmd.msg_text.act_time << ": "; */
    /* cout << tMsgCmd.msg_text.act_precs << ": "; */
    /* cout << tMsgCmd.msg_text.act_min << ": "; */
    /* cout << tMsgCmd.msg_text.act_max << ": "; */
    /* cout << tMsgCmd.msg_text.act_stat << ": "; */
    /* cout << tMsgCmd.msg_text.act_stat_time << endl; */
    /* size_t sizMsg = sizeof(SrvCmd)+sizeof(long); */
    /* MsgCmd tMsgCmd; */
    /* tMsgCmd.msg_type = sizMsg; */
    /* memmove(tMsgCmd.msg_text.sys_id, "666", sizeof("666")); */
    /* tMsgCmd.msg_text.ins_seq_no = 2000; */
    /* tMsgCmd.msg_text.dev_no = 2; */
    /* memmove(tMsgCmd.msg_text.op_code, "wr_ee", sizeof("wr_ee")); */
    /* memmove(tMsgCmd.msg_text.dev_id, "34bca6012345", sizeof("34bca6012345")); */
    /* tMsgCmd.msg_text.ctrl_no = 6; */
    /* memmove(tMsgCmd.msg_text.ctrl_id, "1a", sizeof("1a")); */
    /* memmove(tMsgCmd.msg_text.ctrl_typ, "500", sizeof("500")); */
    /* tMsgCmd.msg_text.act_no = 3; */
    /* memmove(tMsgCmd.msg_text.act_typ, "relay", sizeof("relay")); */
    /* memmove(tMsgCmd.msg_text.act_val, "1", sizeof("1")); */
    /* memmove(tMsgCmd.msg_text.act_unit, "mV", sizeof("mV")); */
    /* tMsgCmd.msg_text.act_time = 01234567; */
    /* tMsgCmd.msg_text.act_precs = 0.005; */
    /* tMsgCmd.msg_text.act_min = 0.000001; */
    /* tMsgCmd.msg_text.act_max= 220.999; */
    /* memmove(tMsgCmd.msg_text.act_stat, "RelayOn", sizeof("RelayOn")); */
    /* tMsgCmd.msg_text.act_stat_time =76543210; */
    /* msgsnd.send_msg(tMsgCmd, sizMsg); */

    /* cout << tMsgCmd.msg_type << endl; */
    /* cout << tMsgCmd.msg_text.sys_id << ": "; */
    /* cout << tMsgCmd.msg_text.ins_seq_no << ": "; */
    /* cout << tMsgCmd.msg_text.dev_no << ": "; */
    /* cout << tMsgCmd.msg_text.op_code << ": "; */
    /* cout << tMsgCmd.msg_text.dev_id << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_no << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_id << ": "; */
    /* cout << tMsgCmd.msg_text.ctrl_typ << ": "; */
    /* cout << tMsgCmd.msg_text.act_no << ": "; */
    /* cout << tMsgCmd.msg_text.act_typ << ": "; */
    /* cout << tMsgCmd.msg_text.act_val << ": "; */
    /* cout << tMsgCmd.msg_text.act_unit << ": "; */
    /* cout << tMsgCmd.msg_text.act_time << ": "; */
    /* cout << tMsgCmd.msg_text.act_precs << ": "; */
    /* cout << tMsgCmd.msg_text.act_min << ": "; */
    /* cout << tMsgCmd.msg_text.act_max << ": "; */
    /* cout << tMsgCmd.msg_text.act_stat << ": "; */
    /* cout << tMsgCmd.msg_text.act_stat_time << endl; */

    return 0;
}


