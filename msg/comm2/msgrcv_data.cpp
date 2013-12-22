/*
 * =====================================================================================
 *
 *       Filename:  Msgrcv_data.cpp
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
#include "msgrcv_data.h"

namespace {
    using namespace std;

    int msg_qid;
    MsgData msg;
    key_t key;
}

Msg_recv::Msg_recv() : Thread() {
    if ( (key = ftok(".", 'a')) == -1) {
        cerr << "ftok() error:" << strerror(errno) << endl;
    }
    else {
        // create MsgData queue
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
        // create MsgData queue
        if ( (msg_qid = msgget(key, 0600 | IPC_CREAT)) == -1) {
            cerr << "msgget() error:" << strerror(errno) << endl;
        }
        else {
            cout << "Open queue " << msg_qid << endl;
        }
    }
}

Msg_recv::~Msg_recv() {
    // remove MsgData queue
    if ( (msgctl(msg_qid, IPC_RMID, NULL)) < 0) {
        cerr << "msgctl() error:" << strerror(errno) << endl;
    }
    else {
        cout << "IPC receive already removed." << endl;
    }
}

void* Msg_recv::run() {
    while (!is_stop()) {
        // read MsgData queue
        memset(&msg.msg_text, 0, TEXT_SIZE);
        // if ( (msgrcv(msg_qid, &msg, sizeof(msg.msg_text), 0, 0)) < 0) {
        if ( (msgrcv(msg_qid, &msg, TEXT_SIZE, 0, 0)) < 0) {
            cerr << "msgrcv() error:" << strerror(errno) << endl;
        }
        else {
            cache_msg(msg);
            cout << msg.msg_type << endl;
            cout << msg.msg_text << endl;
        }
    }
	// cout << "Receive \"quit\" from client" << endl;

    return 0;
}

