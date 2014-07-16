#include <iostream>
#include "unittest/unittest.h"
#include "lightbench/base/event_loop.h"
#include "lightbench/base/singleton.h"

using namespace lightbench;

TEST(TEST_EVENTLOOP, TEST_1){
    EventLoopMgr& mgr = Singleton<EventLoopMgr>::instance();
}

TEST_START(EVENTLOOP_TEST)
