/*
 * =====================================================================================
 *
 *       Filename:  Msgrcv_cmd.cpp
 *
 *    Description:  Test message receiving with Thread class.
 *
 *        Version:  1.0
 *        Created:  05/25/2013 07:53:29 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  david zhao
 *        Company:  Beijing Ding Qing Soft Co. Ltd.
 *
 * =====================================================================================
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "Thread.h"
#include "msgrcv_cmd.h"

namespace {
    using namespace std;

    int msg_qid;
    MsgCmd msg;
    key_t key;
}

Msg_recv::Msg_recv() : Thread() {
    if ( (key = ftok(".", 'a')) == -1) {
        cerr << "ftok() error:" << strerror(errno) << endl;
    }
    else {
        // create MsgCmd queue
        if ( (msg_qid = msgget(key, 0600 | IPC_CREAT)) == -1) {
            cerr << "msgget() error:" << strerror(errno) << endl;
        }
        else {
            cout << "Open queue " << msg_qid << endl;
        }
    }
}

Msg_recv::Msg_recv(const string& tstr) : Thread() {
    // check if specified file exist
    fstream _file;
    _file.open(tstr.c_str(), ios::in);
    // create the specified file if not exist
    if(!_file) {
        string tstr1 = "touch ";
        tstr1 += tstr;
        system(tstr1.c_str());
    }

    if ( (key = ftok(tstr.c_str(), 'a')) == -1) {
        cerr << "ftok() error:" << strerror(errno) << endl;
    }
    else {
        // create MsgCmd queue
        if ( (msg_qid = msgget(key, 0600 | IPC_CREAT)) == -1) {
            cerr << "msgget() error:" << strerror(errno) << endl;
        }
        else {
            cout << "Open queue " << msg_qid << endl;
        }
    }
}

Msg_recv::~Msg_recv() {
    // remove MsgCmd queue
    if ( (msgctl(msg_qid, IPC_RMID, NULL)) < 0) {
        cerr << "msgctl() error:" << strerror(errno) << endl;
    }
    else {
        cout << "IPC receive already removed." << endl;
    }
}

MsgCmd& Msg_recv::get_cur_msg() {
    static MsgCmd tmsg;

    tmsg = msg_queue.front();
    msg_queue.pop_front();

    return tmsg;
}

void* Msg_recv::run() {
    while (!is_stop()) {
        // read MsgCmd queue
	cout << "recv start" << endl;
        memset(&msg.msg_text, 0, sizeof(msg.msg_text));
        if ( (msgrcv(msg_qid, &msg, sizeof(msg.msg_text)+4, 0, 0)) < 0) {
            cerr << "msgrcv() error:" << strerror(errno) << endl;
            // return -1;
        }
        else {
            cache_msg(msg);
            // msg_queue.push_back(msg);
            // cout << "From process " << msg.msg_type << ": " << msg.msg_text[0] << endl;

        }
	cout << "recv end" << endl;
    }
	// cout << "Receive \"quit\" from client" << endl;

    return 0;
}

