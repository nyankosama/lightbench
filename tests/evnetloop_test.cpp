#include <iostream>
#include "unittest/unittest.h"
#include "lightbench/base/event_loop.h"
#include "muduo/base/Singleton.h"

using namespace lightbench;
using namespace muduo;

TEST(TEST_EVENTLOOP, TEST_1){
    EventLoopMgr& mgr = Singleton<EventLoopMgr>::instance();
}

TEST_START(EVENTLOOP_TEST)
