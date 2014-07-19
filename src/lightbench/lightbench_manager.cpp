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
#include "lightbench/lightbench_manager.h"
#include "muduo/base/Singleton.h"

using namespace lightbench;

LightbenchManager::LightbenchManager(const std::string& host, int port, int coreNum)
    :
    count_(0),
    host_(host),
    port_(port),
    coreNum_(coreNum) {}


void LightbenchManager::benchmark(
    int reqNum,
    const std::string& data) {

    std::cout << "thread start" << std::endl;

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host_.c_str());
    addr.sin_port = htons(port_);
    int len = sizeof(addr);

    char readbuf[1024];


    for (int i = 0; i < reqNum; i++) {
        int count = count_.fetch_add(1);
        std::string dataToSend = data + boost::lexical_cast<std::string>(count);
        const char* buf = dataToSend.c_str();
        int dataLen = dataToSend.size();

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
        std::cout << "send data to server, id=" << count << std::endl;

        ret = read(clientfd, readbuf, sizeof(readbuf));
        if (ret == -1) {
            std::cerr <<  "client read error: " << strerror(errno) << std::endl;
            abort();
        }

        std::cout << "read data from server, id=" << count << std::endl;
        close(clientfd);
    }
}

int getSplitReqNum(int index, int totalReqNum, int coreNum) {
    if (index == coreNum - 1) {
        return  (totalReqNum % coreNum) + (totalReqNum / coreNum);
    } else {
        return  totalReqNum / coreNum;
    }
}

void LightbenchManager::startBench(const std::string& data, int totalReqNum) {
    ThreadVector threadList;

    for (int i = 0; i < coreNum_; i++) {
        boost::function<void ()> func = boost::bind(&LightbenchManager::benchmark, this, getSplitReqNum(i, totalReqNum, coreNum_), data);
        std::shared_ptr<muduo::Thread> thread(new muduo::Thread(func));
        threadList.push_back(thread);
        thread->start();
    }

    for (auto thread : threadList) {
        thread->join();
        std::cout << "thread join return" << std::endl;
    }
}

void LightbenchManager::report() {
    //TODO
}

