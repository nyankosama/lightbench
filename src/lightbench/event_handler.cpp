#include <iostream>
#include <error.h>
#include "lightbench/event_handler.h"

using namespace lightbench;

void ReadServerHandler::handleEvent(EventType type) {
    if (type == EventType::EVENT_CLOSE) {
        mgr_->removeHandler(sockfd_);
        close(sockfd_);
        //pvQueue_->put(1);
        return;
    }

    int n = 0;
    char buf[512];
    while ((n = read(sockfd_, buf, sizeof(buf))) != 0) {
        if (n == -1) {
            if (errno == EAGAIN) {
                break;
            }
            std::cerr << "echo read error! err=" << strerror(errno) << std::endl;
            break;
            //abort();
        }
        
        std::cout << "receive from clientfd =" << sockfd_ << ", str=" << buf << std::endl;

    }
    //close(sockfd_);
    mgr_->removeHandler(sockfd_);
    //pvQueue_->put(1);
}


