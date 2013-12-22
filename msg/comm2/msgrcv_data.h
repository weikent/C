/*
 * =====================================================================================
 *
 *       Filename:  msgrcv_data.h
 *
 *    Description:  Header file of Msgrcv_data.cpp
 *
 *        Version:  1.0
 *        Created:  06/22/2013 04:22:17 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  david.zhao
 *        Company:  Beijing Ding Qing Soft Co. Ltd.
 *
 * =====================================================================================
 */
#ifndef __MSGRECV_H_
#define __MSGRECV_H_

#include <deque>
#include "Thread.h"
#include "message.h"

using std::string;

class Msg_recv : public Thread {
public:
    Msg_recv();
    Msg_recv(const string&);
    virtual ~Msg_recv();
    virtual void* run();
    void cache_msg(const MsgData& new_msg) {
        msg_queue.push_back(new_msg);
    }
    MsgData& get_cur_msg() {
        static MsgData tmsg;

        tmsg = msg_queue.front();
        msg_queue.pop_front();

        return tmsg;
    }
    bool msg_is_empty() {
        return msg_queue.empty();
    }
private:
    std::deque<MsgData> msg_queue;
};

#endif //__MSGRECV_H_

