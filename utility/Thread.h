#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread {
public:
    Thread();
    virtual ~Thread();
    void start();
    void stop();
    virtual void* run() = 0;
    // bool const is_stop() const;
    bool is_stop() const;
    void* get_owner() const;
	pthread_t get_thread_id() const;
    void set_owner(void* arg);
private:
    pthread_t thread_id_;
    void* owner_;
    bool volatile is_quit_;
    int retval_pthrd_create;
};

#endif //__THREAD_H__
