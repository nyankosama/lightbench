#include <iostream>
#include <boost/function.hpp>
#include "third_party/unittest/unittest.h"
#include "lightbench/base/thread.h"

using namespace lightbench;

void threadFunc(ThreadData data){
    std::cout << data.name_ << " is running" << std::endl;
}

TEST(TEST_THREAD, TEST_1){
    boost::function<void (ThreadData)> func = &threadFunc;
    Thread th(func, "test_thread");
    th.start();
    th.join();
}

TEST_START(THREAD_TEST)
