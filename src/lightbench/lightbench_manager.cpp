#include <sys/types.h>
#include <sys/socket.h>
#include "lightbench/lightbench_manager.h"
#include "lightbench/base/thread.h"

using namespace lightbench;

LightbenchManager::LightbenchManager(const std::string& host, const std::string& port, int coreNum)
    :host_(host),port_(port),coreNum_(coreNum) {
         
    }

std::shared_ptr<EventLoopMgr> createEventLoopMgr(){
    //TODO
    return NULL;
}

void connectFunc(ThreadData data){
    int sockfd = *((int*)data.data_);
}

void eventLoop(ThreadData data){

}


int LightbenchManager::startBench(const std::string& data, int concurrentNum, int reqNum){
    //TODO
    return 0;
}

void LightbenchManager::report(){
    //TODO
}
