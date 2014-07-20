#ifndef _LIGHTBENCH_MGR_H_
#define _LIGHTBENCH_MGR_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>
#include <atomic>
#include "muduo/base/BlockingQueue.h"
#include "muduo/base/Thread.h"
#include "muduo/base/Mutex.h"

namespace lightbench {

    class LightbenchManager : boost::noncopyable{
    public:
        typedef std::vector<std::shared_ptr<muduo::Thread> > ThreadVector; 
    public:
        LightbenchManager(const std::string& host, int port, int coreNum);
        virtual ~LightbenchManager() {}

        void startBench(const std::string& data, int totalReqNum);
        void report();

        //the following functions will be called by thread automatically
        void benchmark(int reqNum, const std::string& data);

    private:
        void initPvQueue(int concurrentNum);
        void record(int costTime, int reqNum);

    private:
        std::string host_;
        int port_;
        int coreNum_;
        int totalReqNum_;

    private:
        //statistic
        std::atomic_int minRespTime_;
        std::atomic_int maxRespTime_;
        std::atomic_int totalRespTime_;
        std::atomic_int completedCount_;
        int totalPassTime_;
        muduo::MutexLock lock_;
    };

}

#endif

