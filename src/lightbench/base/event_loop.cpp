#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "event_loop.h"

using namespace lightbench;

EventLoopMgr::EventLoopMgr():isAcitive_(true) {
    events_ = new epoll_event[MAX_EVENTS];
    efd_ = epoll_create(1);
}

EventLoopMgr::~EventLoopMgr() {
    delete events_;
}

int EventLoopMgr::handleEvent(int timeout) {
    if (!isAcitive_ && handlerTable_.size() == 0){
        std::cout << "handleEvent return" << std::endl; 
        return FAIL;
    }
    int n;
    std::cout << "epoll_wait start, table size=" << handlerTable_.size() << std::endl;
    n = epoll_wait(efd_, events_, MAX_EVENTS, timeout);
    std::cout << "epoll_wait return, n=" << n << ", table size =" << handlerTable_.size() <<std::endl;
    if (n == 0){
        //timeout
        std::cout << "mgr timeout!, table size=" << handlerTable_.size() << std::endl;
        /*for (mapIter iter = handlerTable_.begin(); iter != handlerTable_.end(); ++iter){ 
            iter->second->handleEvent(EventType::EVENT_CLOSE);
        }*/
        return SUCCESS;
    }
    for (int i = 0; i < n; i++) {
        if ((events_[i].events & EPOLLERR) ||
                (events_[i].events & EPOLLHUP) ||
                (!(events_[i].events & EPOLLIN)))  {
            std::cerr << "epoll_wait error" << std::endl;
            std::cerr << "events=" << events_[i].events << std::endl;
            abort();
        }

        std::shared_ptr<EventHandler> handler = handlerTable_[events_[i].data.fd];
        handler->handleEvent(EVENT_READ);
    }
    return SUCCESS;
}

void EventLoopMgr::registerHandler(std::shared_ptr<EventHandler>& handler) {
    muduo::MutexLockGuard lock(mutexLock_);
    handlerTable_[handler->getSockfd()] = handler;
    int ret = epoll_ctl(efd_, EPOLL_CTL_ADD, handler->getSockfd(), &(handler->getEvent()));
    if (ret == -1) {
        std::cerr << "epoll_ctl_add error!" << std::endl;
        abort();
    }
}

void EventLoopMgr::removeHandler(int keyfd) {
    muduo::MutexLockGuard lock(mutexLock_);
    int ret = epoll_ctl(efd_, EPOLL_CTL_DEL, keyfd, NULL);
    if (ret == -1) {
        std::cerr << "epoll_ctl_del error!" << std::endl;
        abort();
    }

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

int lightbench::makeSocketSync(int sfd){
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1) {
        perror ("fcntl");
        return -1;
    }

    flags |= O_SYNC;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
        perror ("fcntl");
        return -1;
    }

    return 0;
}
