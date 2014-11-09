#include <iostream>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include "lightbench/bench_manager.h"

namespace po = boost::program_options;
using namespace lightbench;

int main(int argc, char* argv[]) {

    std::string host;
    int port;
    std::string data;
    int requestNum;
    int coreNum = 1;

    po::options_description desc("Required options");

    desc.add_options()
    ("help", "produce help meesage")
    ("host,h", po::value<std::string>(&host), "host ip")
    ("port,p", po::value<int>(&port), "port")
    ("data,d", po::value<std::string>(&data), "sended data")
    ("num,n", po::value<int>(&requestNum), "the number of request to send");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm); 

    if (vm.count("help") || vm.empty()) {
        std::cout << desc << std::endl;
        exit(0);
    }

    if (vm.size() != 4){
        std::cout << "Invaid arguments!" << std::endl;
        std::cout << std::endl;
        std::cout << desc << std::endl;
        exit(0);
    }
    
    LightbenchManager mgr(host, port, coreNum);
    mgr.startBench(data, requestNum);
    mgr.report();
}
