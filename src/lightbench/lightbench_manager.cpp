#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <boost/bind.hpp>
#include <assert.h>
#include <unistd.h>
#include "lightbench/lightbench_manager.h"
#include "muduo/base/Singleton.h"
#include "lightbench/base/event_loop.h"
#include "lightbench/event_handler.h"

using namespace lightbench;

void eventLoop(LightbenchManager::mgrPtr mgr) {
    int ret = 0;
    while (true){
        ret = mgr->handleEvent(5000);
        if (ret == 0){
            std::cout << "complete!" << std::endl;
            break; 
        }
    }
}

std::shared_ptr<EventHandler> createReadServerHandler(int sockfd, std::shared_ptr<EventLoopMgr> mgr, LightbenchManager::pvQueuePtr pvQueue){
    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    std::shared_ptr<EventHandler> hdr(new ReadServerHandler(sockfd, event, mgr, pvQueue));
    return hdr;
}

LightbenchManager::LightbenchManager(const std::string& host, int port, int coreNum)
    :host_(host),
     port_(port),
     coreNum_(coreNum),
     pvQueue_(new muduo::BlockingQueue<int>()) {}

void LightbenchManager::initPvQueue(int concurrentNum) {
    pvQueue_ = pvQueuePtr(new muduo::BlockingQueue<int>());
    for (int i = 0; i < concurrentNum; i++)
        pvQueue_->put(1);

    assert(pvQueue_->size() == (size_t)concurrentNum);    
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
        pvQueue_->take();
        int clientfd = socket(AF_INET, SOCK_STREAM, 0);
        int ret = connect(clientfd, (struct sockaddr *)&addr, len);
        if (ret == -1) {
            std::cerr <<  "connect error: " << strerror(errno) << std::endl;
            abort();
        }
        //makeSocketNonBlocking(clientfd);
        //makeSocketSync(clientfd);
        
        mgrPtr mgr = mgrTable_[i%coreNum_];

        std::shared_ptr<EventHandler> hdr = createReadServerHandler(clientfd, mgr, pvQueue_);
        mgr->registerHandler(hdr);

        const char* buf = data.c_str();
        int dataLen = data.size();
        ret = send(clientfd, buf, dataLen, 0);
        if (ret != dataLen) {
            std::cerr <<  "client write error: " << strerror(errno) << std::endl;
            abort();
        }
        std::cout << "send data to server, sockfd=" << clientfd << std::endl;

    }
    for (auto mgr : mgrTable_){
        mgr->disable(); 
    }
}

int LightbenchManager::startBench(const std::string& data, int concurrentNum, int reqNum) {
    threadVector threadList;
    for (int i = 0; i < coreNum_; i++){
        mgrPtr mgr(new EventLoopMgr());
        mgrTable_.push_back(mgr);
        boost::function<void ()> eventFunc = boost::bind(eventLoop, mgr);
        std::shared_ptr<muduo::Thread> thread(new muduo::Thread(eventFunc));
        threadList.push_back(thread);
        std::cout << "thread start" << std::endl;
        thread->start();
    }

    boost::function<void ()> connectFunc = boost::bind(&LightbenchManager::connectServer, this, concurrentNum, reqNum, data);
    std::shared_ptr<muduo::Thread> connectThread(new muduo::Thread(connectFunc));
    connectThread->start();

    //thread join
    std::cout << "thread join" << std::endl;
    connectThread->join();
    for (auto thread : threadList){
        thread->join();
    }
    std::cout << "join completed" << std::endl;
     
    return SUCCESS;
}
;
void LightbenchManager::report() {
    //TODO
}

