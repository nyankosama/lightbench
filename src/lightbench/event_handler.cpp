#include <iostream>
#include <string.h>
#include "event_handler.h"

using namespace lightbench;

std::shared_ptr<EventHandler> EventHandlerFactory::createHandler(HandlerType type, int sockfd) {
    struct epoll_event event;
    switch (type) {
    case READ_PRINT_HANDLER:
        event.events = EPOLLIN | EPOLLET;
        return std::shared_ptr<ReadPrintHandler>(new ReadPrintHandler(sockfd, event));
    }
    return NULL;
}

void ReadPrintHandler::handleEvent(EventType type) {
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
        buf[n] = '\n';
        buf[n+1] = '\0';
        std::cout << buf << std::endl;
    }

    close(sockfd_);
}

