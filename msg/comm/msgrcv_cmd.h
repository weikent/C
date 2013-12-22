/*
 * =====================================================================================
 *
 *       Filename:  msgrcv_cmd.h
 *
 *    Description:  Header file of Msgrcv_cmd.cpp
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
    void cache_msg(const MsgCmd& new_msg);
    MsgCmd& get_cur_msg();
    bool msg_is_empty();
private:
    std::deque<MsgCmd> msg_queue;
};

inline void Msg_recv::cache_msg(const MsgCmd& new_msg) {
    msg_queue.push_back(new_msg);
}

inline bool Msg_recv::msg_is_empty() {
    return msg_queue.empty();
}

#endif //__MSGRECV_H_

