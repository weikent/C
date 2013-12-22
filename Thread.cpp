#include <iostream>
#include "Thread.h"

namespace {
    using namespace std;

    void* thread_func(void* arg) {
        Thread* pThis = static_cast<Thread*>(arg);
        pThis->run();
	cout << "Return from pThis->run()" << endl;
	pthread_detach(pThis->get_thread_id());
	cout << "Detached from parent thread" << endl;
	return NULL;
    }
}

Thread::Thread() : owner_(0), is_quit_(false), retval_pthrd_create(0) {
}

Thread::~Thread() {
    // pthread_cancel(thread_id_);
}

void Thread::start() {
    retval_pthrd_create = pthread_create(&thread_id_, NULL, thread_func, this);
}

// void* Thread::run() {
//     return 0;
// }

void Thread::stop() {
    is_quit_ = true;
}

// bool const Thread::is_stop() const {
bool Thread::is_stop() const {
    return is_quit_;
}

pthread_t Thread::get_thread_id() const {
    return thread_id_;
}

void* Thread::get_owner() const {
    return owner_;
}

void Thread::set_owner(void* arg) {
    owner_ = arg;
}

