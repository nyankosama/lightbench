#include <iostream>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include "lightbench/lightbench_manager.h"

namespace po = boost::program_options;
using namespace lightbench;

int main(int argc, char* argv[]) {

    std::string host = "127.0.0.1";
    int port = 9123;
    std::string data = "haha";
    int concurrent = 100;
    int requestNum = 10;
    int coreNum = 1;

    po::options_description desc("Required options");

    desc.add_options()
    ("help", "produce help meesage")
    ("host,h", po::value<std::string>(&host), "host ip")
    ("port,p", po::value<int>(&port), "port")
    ("data,d", po::value<std::string>(&data), "sended data")
    ("concurrent,c", po::value<int>(&concurrent), "concurrent connections")
    ("num,n", po::value<int>(&requestNum), "the number of request to send");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm); 

    /*if (vm.count("help") || vm.empty()) {
        std::cout << desc << std::endl;
        exit(0);
    }

    if (vm.size() != 5){
        std::cout << "Invaid arguments!" << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;
        exit(0);
    }*/
    
    std::cout << "create manager" << std::endl;
    LightbenchManager mgr(host, port, coreNum);
    mgr.startBench(data, concurrent, requestNum);
    std::cout << "complete!" << std::endl;
}
