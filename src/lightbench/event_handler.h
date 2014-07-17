#include "lightbench/base/event_loop.h"
#include "muduo/base/BlockingQueue.h"

namespace lightbench {

    class ReadServerHandler : public EventHandler {

    public:
        typedef std::shared_ptr<EventLoopMgr> mgrPtr;
        typedef std::shared_ptr<muduo::BlockingQueue<int> > pvQueuePtr;

    public:
        ReadServerHandler(
            int sockfd,
            epoll_event event,
            mgrPtr mgr,
            pvQueuePtr pvQueue
        ):EventHandler(sockfd,event), mgr_(mgr), pvQueue_(pvQueue) {}
        virtual ~ReadServerHandler() {}
        void virtual handleEvent(EventType type);

    private:
        mgrPtr mgr_;
        pvQueuePtr pvQueue_;
    };
}


