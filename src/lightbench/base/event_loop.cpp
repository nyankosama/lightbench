#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "event_loop.h"
#include "singleton.h"

using namespace lightbench;

EventLoopMgr::EventLoopMgr() {
    events_ = new epoll_event[MAX_EVENTS];
    efd_ = epoll_create(1);
}

EventLoopMgr::~EventLoopMgr() {
    delete events_;
}

void EventLoopMgr::handleEvent() {
    int n;
    n = epoll_wait(efd_, events_, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++) {
        if ((events_[i].events & EPOLLERR) ||
                (events_[i].events & EPOLLHUP) ||
                (!(events_[i].events & EPOLLIN)))  {
            std::cerr << "epoll_wait error" << std::endl;
            abort();
        }

        std::shared_ptr<EventHandler> handler = handlerTable_[events_[i].data.fd];
        handler->handleEvent(EVENT_READ);
    }
}

void EventLoopMgr::registerHandler(std::shared_ptr<EventHandler>& handler) {
    handlerTable_[handler->getSockfd()] = handler;
    int ret = epoll_ctl(efd_, EPOLL_CTL_ADD, handler->getSockfd(), &(handler->getEvent()));
    if (ret == -1) {
        std::cerr << "epoll_ctl_add error!" << std::endl;
        abort();
    }
}

void EventLoopMgr::removeHandler(int keyfd) {
    handlerTable_.erase(keyfd);
}


int lightbench::makeSocketNonBlocking(int sfd) {
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1) {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

