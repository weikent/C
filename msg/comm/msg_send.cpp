/*
 * =====================================================================================
 *
 *       Filename:  msg_send.cpp
 *
 *    Description:  Package message sending in class
 *
 *        Version:  1.0
 *        Created:  06/26/2013 02:51:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david.zhao
 *        Company:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg_send.h"

namespace {
    using namespace std;

    int msg_qid;
    key_t key;
}

Msg_send::Msg_send() {
    // check if specified file exist
    if ( (key = ftok(".", 'a')) == -1) {
        cerr << "ftok() error:" << strerror(errno) << endl;
    }
    else {
        // create message queue
        if ( (msg_qid = msgget(key, 0600 | IPC_CREAT)) == -1) {
            cerr << "msgget() error:" << strerror(errno) << endl;
        }
        else {
            cout << "Open queue " << msg_qid << endl;
        }
    }
}

Msg_send::Msg_send(const string& tstr) {
    // check if specified file exist
    fstream _file;
    _file.open(tstr.c_str(), ios::in);
    if(!_file) {
        string tstr1 = "touch ";
        tstr1 += tstr;
        system(tstr1.c_str());
    }

    // if ( (key = ftok(".", 'a')) == -1) {
    if ( (key = ftok(tstr.c_str(), 'a')) == -1) {
        cerr << "ftok() error:" << strerror(errno) << endl;
    }
    else {
        // create message queue
        if ( (msg_qid = msgget(key, 0600 | IPC_CREAT)) == -1) {
            cerr << "msgget() error:" << strerror(errno) << endl;
        }
        else {
            cout << "Open queue " << msg_qid << endl;
        }
    }
}

Msg_send::~Msg_send() {
    // remove message queue
    // if ( (msgctl(msg_qid, IPC_RMID, NULL)) < 0) {
    //     perror("msgctl error");
    // }
    // cout << "Remove queue sending message." << endl;
}

int Msg_send::send_msg(const Message& msg_snd, const int size) {
    if ( (msgsnd(msg_qid, &msg_snd, size, 0)) < 0) {
        perror("message posted error");
    
        return -1;
    }

    return 0;
}

