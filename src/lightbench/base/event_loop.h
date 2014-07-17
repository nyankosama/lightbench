#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_
#include <unistd.h>
#include <memory>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <boost/function.hpp>

#define MAX_EVENTS 512

namespace lightbench {

    enum EventType {
        EVENT_ACCEPT,
        EVENT_CLOSE,
        EVENT_READ,
        EVENT_WRITE,
        EVENT_ERROR
    };

    class EventHandler {
    public:
        EventHandler(int sockfd, epoll_event event)
            :sockfd_(sockfd),
             event_(event){
            event_.data.fd = sockfd;
        }
        virtual ~EventHandler() {};
        void virtual handleEvent(EventType type) = 0;
        epoll_event& getEvent() {
            return event_;
        }
        int getSockfd() const {
            return sockfd_;
        }

    protected:
        int sockfd_;
        epoll_event event_;
    };

    //not thread-safe
    class EventLoopMgr {
    public:
        EventLoopMgr();
        virtual ~EventLoopMgr();
        void handleEvent();
        void registerHandler(std::shared_ptr<EventHandler>& handler);
        void removeHandler(int keyfd);

    private:
        epoll_event*  events_;
        int efd_;
        std::map<int, std::shared_ptr<EventHandler> > handlerTable_;
    };

    //utils
    int makeSocketNonBlocking(int sfd);
}

#endif
