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

    private:
        std::atomic_int count_;
        std::string host_;
        int port_;
        int coreNum_;
    };

}

#endif

