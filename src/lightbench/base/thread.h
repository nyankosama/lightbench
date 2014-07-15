#ifndef _THREAD_H_
#define _THREAD_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include <pthread.h>
#include <iostream>
#include <string>
#include <atomic>

namespace lightbench {
    struct ThreadData;

    class Thread : boost::noncopyable {
    public:
        typedef boost::function<void (ThreadData)> ThreadFunc;

        explicit Thread(const ThreadFunc&, const std::string& name = std::string());
        ~Thread();

        void start();
        int join();

        pid_t pid() const {
            return *tid_;
        }
        const std::string& name() const {
            return name_;
        }

    private:
        void setDefaultName();
        void runInThread();
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        std::shared_ptr<pid_t> tid_;
        ThreadFunc func_;
        std::string name_;
        static std::atomic_int numCreated_;
    };

    struct ThreadData {
        typedef lightbench::Thread::ThreadFunc ThreadFuc;
        ThreadFuc func_;
        std::string name_;
        std::shared_ptr<pid_t> tid_;

        ThreadData(const ThreadFuc& func,
                   const std::string& name,
                   const std::shared_ptr<pid_t>& tid)
            : func_(func),
              name_(name),
              tid_(tid) {}

        void runInThread();
    };

}
#endif


