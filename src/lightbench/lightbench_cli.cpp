#include <iostream>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include "lightbench/base/thread.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {

    std::string host;
    std::string port;
    std::string data;
    int concurrent;
    int requestNum;

    po::options_description desc("Required options");

    desc.add_options()
    ("help", "produce help meesage")
    ("host,h", po::value<std::string>(&host), "host ip")
    ("port,p", po::value<std::string>(&port), "port")
    ("data,d", po::value<std::string>(&data), "sended data")
    ("concurrent,c", po::value<int>(&concurrent), "concurrent connections")
    ("num,n", po::value<int>(&requestNum), "the number of request to send");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm); 

    if (vm.count("help") || vm.empty()) {
        std::cout << desc << std::endl;
        exit(0);
    }

    if (vm.size() != 5){
        std::cout << "Invaid arguments!" << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;
        exit(0);
    }

    //TODO one thread for create connections, corenum threads for eventloop of reading echo data from server.
}
