/*
 * =====================================================================================
 *
 *       Filename:  msgsnd_data.h
 *
 *    Description:  Header file of msg_data.cpp
 *
 *        Version:  1.0
 *        Created:  06/26/2013 02:52:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david.zhao
 *        Company:  Beijing Ding Qing Soft Co. Ltd.
 *
 * =====================================================================================
 */
#ifndef __MSGSEND_H_
#define __MSGSEND_H_

#include "message.h"

using std::string;

class Msg_send {
public:
    Msg_send();
    Msg_send(const string&);
    ~Msg_send();
    int send_msg(const MsgData& msg_snd, const int size);
private:
};

#endif //__MSGSEND_H_

