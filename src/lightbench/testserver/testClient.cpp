#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <third_party/muduo/base/Thread.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>
#include "lightbench/bench_utils.h"

void runnable(){
    int server_sockfd;
    socklen_t server_len;
    struct sockaddr_in server_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(9123);
    server_len = sizeof(server_address);

    connect(server_sockfd, (struct sockaddr *)&server_address, server_len);

    char data[128];
    for (int i = 0; i < 200000; i++){
        sprintf(data, "hello world");
        write(server_sockfd, data, 12);
        read(server_sockfd, data, 128);
    }
    close(server_sockfd);
}

int main(){
    boost::function<void ()> threadFunc = boost::bind(runnable);
    std::vector<std::shared_ptr<muduo::Thread> > list;
    int begin = lightbench::getCurrentTimeUsec();
    for (int i = 0; i < 5; i++){
        std::shared_ptr<muduo::Thread> ptr(new muduo::Thread(threadFunc));
        list.push_back(ptr); 
        ptr->start();
    }
    for (auto item : list){
        item->join();  
    }
    int end = lightbench::getCurrentTimeUsec();
    int cost = (end - begin) / 1000;
    std::cout << "cost time:" << cost << "ms, qps:" << 1000000 / cost * 1000 << std::endl;
}
