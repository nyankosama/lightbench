#ifndef _LIGHTBENCH_MGR_H_
#define _LIGHTBENCH_MGR_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "lightbench/base/event_loop.h"
#include "muduo/base/BlockingQueue.h"

namespace lightbench {

    class LightbenchManager {
    public:
        typedef std::shared_ptr<EventLoopMgr> mgrPtr;
        typedef std::shared_ptr<muduo::BlockingQueue<int> > pvQueuePtr;

    public:
        LightbenchManager(const std::string& host, int port, int coreNum);
        virtual ~LightbenchManager() {}

        int startBench(const std::string& data, int concurrentNum, int reqNum);
        void report();

    private:
        void connectServer(int concurrentNum, int reqNum, const std::string& data);
        void eventLoop();
        void initPvQueue(int concurrentNum);

    private:
        std::string host_;
        int port_;
        std::vector<mgrPtr> mgrTable_;
        int coreNum_;
        pvQueuePtr pvQueue_; 
    };

}

#endif

