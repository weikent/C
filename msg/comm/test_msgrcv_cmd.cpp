/*
 * =====================================================================================
 *
 *       Filename:  test_msgrcv.cpp
 *
 *    Description:  Test to use msg_recv class.
 *
 *        Version:  1.0
 *        Created:  06/28/2013 10:13:34 AM
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
#include <string>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "msgrcv_cmd.h"

namespace {
    using namespace std;

    Msg_recv* msgrecv_thrd = NULL;
}

void destroy_msgrecv_thrd () {
    msgrecv_thrd->stop();
    cout << "msgrecv_thrd->stop() is called." << endl;
    delete msgrecv_thrd;
}

// void destroy_keyin_thrd() {
//     keyin_thrd->stop();
//     cout << "keyin_thrd->stop() is called." << endl;
//     delete keyin_thrd;
// }

int main() {
	string tstr;
    MsgCmd tmsg;

    msgrecv_thrd = new Msg_recv("tpt_snd_ctl_rcv");
    atexit(destroy_msgrecv_thrd);
    cout << "Msg_recv created" << endl;
    msgrecv_thrd->start();
    cout << "Msg_recv started" << endl;
    // keyin_thrd = new Key_input();
    // cout << "Key_input created" << endl;
    // keyin_thrd->start();
    // cout << "Key_input started" << endl;
    // atexit(destroy_keyin_thrd);

    int flag = fcntl(0, F_GETFL, 0);
    flag |= O_NONBLOCK;
    if (fcntl(0, F_SETFL, flag) < 0) {      /* fgetc no-block now */
        cerr << "fcntl() error: Set stdin to non-block fails." << strerror(errno) << endl;
        // perror("Set stdin to non-block fails.");
        // exit(1);
    }

//    sleep(20);
    // while(cin >> tstr) {
    int i = 0;
    while(1) {
    // char tchar[] = "hello world";
    // while(!keyin_thrd->keyin_stop()) {
        if (!msgrecv_thrd->msg_is_empty()) {
            tmsg = msgrecv_thrd->get_cur_msg();
	    cout << "i = " << i<< endl;
	    i++;
            // cout << "Inside of while, " << tmsg.msg_type << ": " << tmsg.msg_text << endl;
            // cout.setf(ios_base::hex, ios_base::hex);
            // cout << "Inside of while, " << tmsg.msg_type << ": " << tmsg.msg_text << endl;
            // printf("Inside of while: %x, %x\n", (unsigned int)tmsg.msg_type, (unsigned int)tmsg.msg_text[0]);
            cout << tmsg.msg_text.sys_id << ": ";
            cout << tmsg.msg_text.ins_seq_no << ": ";
            cout << tmsg.msg_text.dev_no << ": ";
            cout << tmsg.msg_text.op_code << ": ";
            cout << tmsg.msg_text.dev_id << ": ";
            cout << tmsg.msg_text.ctrl_no << ": ";
            cout << tmsg.msg_text.ctrl_id << ": ";
            cout << tmsg.msg_text.ctrl_typ << ": ";
            cout << tmsg.msg_text.act_no << ": ";
            cout << tmsg.msg_text.act_typ << ": ";
            cout << tmsg.msg_text.act_val << ": ";
            cout << tmsg.msg_text.act_unit << ": ";
            cout << tmsg.msg_text.act_time << ": ";
            cout << tmsg.msg_text.act_precs << ": ";
            cout << tmsg.msg_text.act_min << ": ";
            cout << tmsg.msg_text.act_max << ": ";
            cout << tmsg.msg_text.act_stat << ": ";
            cout << tmsg.msg_text.act_stat_time << endl;
            memset(&tmsg.msg_text, 0, sizeof(tmsg.msg_text));
        }
        // usleep(200000);
        // if (cin >> tstr) {
        //     break;
        // }
        if ( getc(stdin) == 'x') {
            break;
        }
    }
    // while (!msgrecv_thrd->msg_is_empty()) {
    //     // tmsg = msg_queue.front();
    //     tmsg = msgrecv_thrd->get_cur_msg();
    //     cout << "Outside of while, " << tmsg.msg_type << ": " << tmsg.msg_text << endl;
    //     // msg_queue.pop_front();
    // }
    // cout << "msgrecv_thrd finished normally." << endl;

    return 0;
}


