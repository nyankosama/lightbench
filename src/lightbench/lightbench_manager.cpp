#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include "lightbench/lightbench_manager.h"
#include "muduo/base/Thread.h"
#include "muduo/base/Singleton.h"
#include "lightbench/base/event_loop.h"
#include "lightbench/event_handler.h"

using namespace lightbench;

LightbenchManager::LightbenchManager(const std::string& host, int port, int coreNum)
    :host_(host),
     port_(port),
     coreNum_(coreNum),
     pvQueue_(new muduo::BlockingQueue<int>()) {
    for (int i = 0; i < coreNum_; i++) {
        mgrPtr mgr(new EventLoopMgr());
        mgrTable_.push_back(mgr);
    }
}

void LightbenchManager::initPvQueue(int concurrentNum) {
    pvQueue_ = pvQueuePtr(new muduo::BlockingQueue<int>());
    for (int i = 0; i < concurrentNum; i++)
        pvQueue_->put(1);
}

void LightbenchManager::connectServer(
    int concurrentNum,
    int reqNum,
    const std::string& data) {

    initPvQueue(concurrentNum);
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host_.c_str());
    addr.sin_port = htons(port_);
    int len = sizeof(addr);

    for (int i = 0; i < reqNum; i++) {
        int clientfd = socket(AF_INET, SOCK_STREAM, 0);
        int ret = connect(clientfd, (struct sockaddr *)&addr, len);
        if (ret == -1) {
            std::cerr <<  "connect error: " << strerror(errno) << std::endl;
            abort();
        }

        mgrPtr mgr = mgrTable_[i%coreNum_];
        std::shared_ptr<EventHandler> hdr = muduo::Singleton<EventHandlerFactory>::instance().createHandler(READ_PRINT_HANDLER, clientfd);
        mgr->registerHandler(hdr);

        const char* buf = data.c_str();
        int dataLen = data.size();
        ret = write(clientfd, buf, dataLen);
        if (ret != dataLen) {
            std::cerr <<  "client write error: " << strerror(errno) << std::endl;
            abort();
        }
    }
}

void LightbenchManager::eventLoop() {

}

int LightbenchManager::startBench(const std::string& data, int concurrentNum, int reqNum) {
    //TODO
    return 0;
}

void LightbenchManager::report() {
    //TODO
}





