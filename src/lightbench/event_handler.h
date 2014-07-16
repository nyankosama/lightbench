#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include "lightbench/base/event_loop.h"

namespace lightbench {

    enum HandlerType {
        READ_PRINT_HANDLER
    };

    class ReadPrintHandler : public EventHandler {
    public:
        ReadPrintHandler(int sockfd, epoll_event event)
            :EventHandler(sockfd, event) {}

        virtual ~ReadPrintHandler() {}
        void virtual handleEvent(EventType type);
    };

    class EventHandlerFactory {
    public:
        std::shared_ptr<EventHandler> createHandler(HandlerType type, int sockfd);
    };
}

#endif
