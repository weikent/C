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
 *        Company:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg_send.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

const int BUF_SIZE = 64;

int main() {
    Msg_send msgsnd("tpt_snd_ctl_rcv");
    // Message tmsg;
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

    // Test binary format message
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
    size_t sizMsg = sizeof(SrvCmd)+sizeof(long);
    Message* pMsg = (Message*)malloc(sizMsg);
    pMsg->msg_type = sizMsg;
    strncpy(pMsg->msg_text[0].devNo, "3", sizeof("3"));
    strncpy(pMsg->msg_text[0].opCode, "wr_ee", sizeof("wr_ee"));
    strncpy(pMsg->msg_text[0].devID, "34bca6012345", sizeof("34bca6012345"));
    strncpy(pMsg->msg_text[0].cntrlActNo, "6", sizeof("6"));
    strncpy(pMsg->msg_text[0].theAct[0].cntrlID, "1a", sizeof("1a"));
    strncpy(pMsg->msg_text[0].theAct[0].cntrlTyp, "500", sizeof("500"));
    strncpy(pMsg->msg_text[0].theAct[0].actID, "3", sizeof("3"));
    strncpy(pMsg->msg_text[0].theAct[0].actVal, "99", sizeof("99"));
    strncpy(pMsg->msg_text[0].theAct[1].cntrlID, "2a", sizeof("2a"));
    strncpy(pMsg->msg_text[0].theAct[1].cntrlTyp, "505", sizeof("505"));
    strncpy(pMsg->msg_text[0].theAct[1].actID, "7", sizeof("7"));
    strncpy(pMsg->msg_text[0].theAct[1].actVal, "66", sizeof("66"));
    strncpy(pMsg->msg_text[0].theAct[2].cntrlID, "3a", sizeof("3a"));
    strncpy(pMsg->msg_text[0].theAct[2].cntrlTyp, "510", sizeof("510"));
    strncpy(pMsg->msg_text[0].theAct[2].actID, "9", sizeof("9"));
    strncpy(pMsg->msg_text[0].theAct[2].actVal, "77", sizeof("77"));
    Message& tmsg = *pMsg;
    msgsnd.send_msg(tmsg, sizMsg);
    free(pMsg);

    cout << pMsg->msg_type << endl;
    cout << pMsg->msg_text[0].devNo << endl;
    cout << pMsg->msg_text[0].opCode << endl;
    cout << pMsg->msg_text[0].devID << endl;
    cout << pMsg->msg_text[0].cntrlActNo << endl;
    cout << pMsg->msg_text[0].theAct[0].cntrlID << endl;
    cout << pMsg->msg_text[0].theAct[0].cntrlTyp << endl;
    cout << pMsg->msg_text[0].theAct[0].actID << endl;
    cout << pMsg->msg_text[0].theAct[0].actVal << endl;
    cout << pMsg->msg_text[0].theAct[1].cntrlID << endl;
    cout << pMsg->msg_text[0].theAct[1].cntrlTyp << endl;
    cout << pMsg->msg_text[0].theAct[1].actID << endl;
    cout << pMsg->msg_text[0].theAct[1].actVal << endl;
    cout << pMsg->msg_text[0].theAct[2].cntrlID << endl;
    cout << pMsg->msg_text[0].theAct[2].cntrlTyp << endl;
    cout << pMsg->msg_text[0].theAct[2].actID << endl;
    cout << pMsg->msg_text[0].theAct[2].actVal << endl;

    return 0;
}

