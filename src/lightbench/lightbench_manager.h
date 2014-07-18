#ifndef _LIGHTBENCH_MGR_H_
#define _LIGHTBENCH_MGR_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>
#include "lightbench/base/event_loop.h"
#include "muduo/base/BlockingQueue.h"
#include "muduo/base/Thread.h"

namespace lightbench {

    class LightbenchManager : boost::noncopyable{
    public:
        typedef std::shared_ptr<EventLoopMgr> mgrPtr;
        typedef std::shared_ptr<muduo::BlockingQueue<int> > pvQueuePtr;
        typedef std::vector<std::shared_ptr<muduo::Thread> > threadVector;

    public:
        LightbenchManager(const std::string& host, int port, int coreNum);
        virtual ~LightbenchManager() {}

        int startBench(const std::string& data, int concurrentNum, int reqNum);
        void report();

        //do not call the following member functions
        void connectServer(int concurrentNum, int reqNum, const std::string& data);

    private:
        void initPvQueue(int concurrentNum);

    private:
        std::string host_;
        int port_;
        std::vector<mgrPtr>  mgrTable_;
        int coreNum_;
        pvQueuePtr pvQueue_; 
    };

}

#endif

