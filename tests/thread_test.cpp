#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "unittest/unittest.h"
#include "muduo/base/Thread.h"


void threadFunc(const char* msg){
    std::cout << "thread msg = " << msg << std::endl;
}

TEST(TEST_THREAD, TEST_1){
    boost::function<void ()> func = boost::bind(&threadFunc, "haha");
    muduo::Thread th(func, "test_thread");
    th.start();
    th.join();
}

TEST_START(THREAD_TEST)
