#include <iostream>
#include "lightbench/event_handler.h"

using namespace lightbench;

void ReadServerHandler::handleEvent(EventType type) {
    if (type == EventType::EVENT_CLOSE) {
        mgr_->removeHandler(sockfd_);
        close(sockfd_);
        pvQueue_->put(1);
    }

    int n = 0;
    char buf[512];
    while ((n = read(sockfd_, buf, sizeof(buf))) != 0) {
        if (n == -1) {
            if (errno == EAGAIN) {
                break;
            }
            std::cerr << "echo read error! err=" << strerror(errno) << std::endl;
            abort();
        }
        
        std::cout << "receive from server, str=" << buf << std::endl;

    }
    mgr_->removeHandler(sockfd_);
    close(sockfd_);
    pvQueue_->put(1);
}


