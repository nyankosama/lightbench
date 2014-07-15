#include <iostream>
#include "third_party/unittest/unittest.h"
#include "lightbench/base/EventLoop.h"
#include "lightbench/base/Singleton.h"

using namespace lightbench;

TEST(TEST_EVENTLOOP, TEST_1){
    EventLoopMgr& mgr = Singleton<EventLoopMgr>::instance();
}

TEST_START(EVENTLOOP_TEST)
