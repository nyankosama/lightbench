#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <unistd.h>
#include <limits>
#include "lightbench/bench_manager.h"
#include "lightbench/bench_utils.h"
#include "muduo/base/Singleton.h"

using namespace lightbench;

LightbenchManager::LightbenchManager(const std::string& host, int port, int coreNum)
    :
    host_(host),
    port_(port),
    coreNum_(coreNum),
    minRespTime_(std::numeric_limits<int>::max()),
    maxRespTime_(0),
    totalRespTime_(0),
    completedCount_(0) {}


void LightbenchManager::benchmark(
    int reqNum,
    const std::string& data) {

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host_.c_str());
    addr.sin_port = htons(port_);
    int len = sizeof(addr);
    char readbuf[1024];

    for (int i = 0; i < reqNum; i++) {
        std::string dataToSend = data + boost::lexical_cast<std::string>(completedCount_);
        const char* buf = dataToSend.c_str();
        int dataLen = dataToSend.size();

        int beginTime = getCurrentTimeUsec();
        int clientfd = socket(AF_INET, SOCK_STREAM, 0);
        int ret = connect(clientfd, (struct sockaddr *)&addr, len);
        if (ret == -1) {
            std::cerr <<  "connect error: " << strerror(errno) << std::endl;
            abort();
        }

        ret = send(clientfd, buf, dataLen, 0);
        if (ret != dataLen) {
            std::cerr <<  "client write error: " << strerror(errno) << std::endl;
            abort();
        }

        ret = read(clientfd, readbuf, sizeof(readbuf));
        if (ret == -1) {
            std::cerr <<  "client read error: " << strerror(errno) << std::endl;
            abort();
        }

        int endTime = getCurrentTimeUsec();
        record(endTime - beginTime);

        close(clientfd);
    }
}

void LightbenchManager::record(int costTime) {
    muduo::MutexLockGuard lockGuard(lock_);
    if (costTime < minRespTime_)
        minRespTime_.exchange(costTime);
    if (costTime > maxRespTime_)
        maxRespTime_.exchange(costTime);

    totalRespTime_.fetch_add(costTime);
    completedCount_.fetch_add(1);

    int splitNum = totalReqNum_ / 10;
    if (splitNum == 0){
        return;
    }
    else {
        if (completedCount_ % splitNum == 0){
            int percent = (completedCount_ / splitNum) * 10;
            std::cout << "complete percent:\t" << percent << "%"  << std::endl;
        }
    }
}


void LightbenchManager::startBench(const std::string& data, int totalReqNum) {
    totalReqNum_ = totalReqNum;
    ThreadVector threadList;
    
    int passBeginTime = getCurrentTimeUsec();
    for (int i = 0; i < coreNum_; i++) {
        boost::function<void ()> func = boost::bind(&LightbenchManager::benchmark, this, getSplitReqNum(i, totalReqNum, coreNum_), data);
        std::shared_ptr<muduo::Thread> thread(new muduo::Thread(func));
        threadList.push_back(thread);
        thread->start();
    }

    std::cout << "benchmark start!" << std::endl;
    std::cout << std::endl;

    for (auto thread : threadList) {
        thread->join();
    }
    int passEndTime = getCurrentTimeUsec();
    totalPassTime_ = passEndTime - passBeginTime;
}

void LightbenchManager::report() {
    std::cout << std::endl;
    std::cout << "Report" << std::endl;
    std::cout << std::endl;
    std::cout << "Cost time(ms):\t" << totalPassTime_ / 1000 << std::endl;
    std::cout << std::endl;
    std::cout << "Complete requests:\t" << completedCount_ << std::endl;
    std::cout << "Connections Times(ms):\t" << std::endl;
    std::cout << "min:\t" << (double)minRespTime_ / 1000 << std::endl;
    std::cout << "max:\t" << (double)maxRespTime_ / 1000 << std::endl;
    std::cout << "mean:\t" << (double)totalRespTime_ / completedCount_ / 1000 << std::endl;
    std::cout << "QPS:\t" << (int)((double)completedCount_ / ((double)totalPassTime_ / 1000000)) << std::endl;
}


