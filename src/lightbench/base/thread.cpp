#include <atomic>
#include "lightbench/base/thread.h"

using namespace lightbench;

namespace lightbench {

    void ThreadData::runInThread() {
        try {
            func_(*this);
        } catch(const std::exception& ex) {
            std::cerr << "thread crash in  " << name_ << std::endl;
            std::cerr << "exception = " <<  ex.what() << std::endl;
            abort();
        } catch(...) {
            std::cerr << "thread crash in  " << name_ << std::endl;
            abort();
        }
    }

    void* startThread(void* obj) {
        ThreadData* data = static_cast<ThreadData*>(obj);
        data->runInThread();
        delete data;
        return NULL;
    }
}

std::atomic_int Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(new pid_t(0)),
      func_(func),
      name_(n) {
    setDefaultName();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if (name_.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        name_ = buf;
    }
}

Thread::~Thread() {
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}


void Thread::start() {
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(func_, name_, tid_);
    if (pthread_create(&pthreadId_, NULL, lightbench::startThread, data)) {
        started_ = false;
        std::cerr << "thread start false in " << name_ << std::endl;
        delete data;
    }
}

int Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

