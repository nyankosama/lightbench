#ifndef _LIGHTBENCH_MGR_H_
#define _LIGHTBENCH_MGR_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "lightbench/base/event_loop.h"

namespace lightbench {

    class LightbenchManager {
    public:
        LightbenchManager(const std::string& host, const std::string& port, int coreNum);
        virtual ~LightbenchManager() {}

        int startBench(const std::string& data, int concurrentNum, int reqNum);
        void report();

    private:
        std::string host_;
        std::string port_;
        std::vector<std::shared_ptr<EventLoopMgr> > mgrTable_;
        int coreNum_;
    };

}

#endif
