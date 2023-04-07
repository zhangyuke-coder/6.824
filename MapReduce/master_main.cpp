#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include "wc.hpp"
#include "master.h"
using namespace std;
std::string readAllFile(std::string filename) {
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    return str;
}
int main(int argc, char* argv[]){
    // if(argc < 2){
    //     cout<<"missing parameter! The format is ./Master pg*.txt"<<endl;
    //     exit(-1);
    // }
    // alarm(10);
    std::string dir = "/home/zyk/VcWorkplace/6.824/MapReduce";
    DIR* d = opendir(dir.c_str());
    if (d == NULL)
    {
            printf("d == NULL");
    }
    std::vector<std::string> targets;
    struct dirent* entry;
    while ( (entry=readdir(d)) != NULL)
    {
        std::string name = entry->d_name;
        if(name.substr(0, 2) == "pg") {
            targets.push_back(dir+ '/' + name);
        }

    }
    rpc_server server(9000, std::thread::hardware_concurrency());
    Master master(targets, 2);
    master.run();
    // server.bind("RegWorker", &Master::RegWorker, &master);
    // server.bind("GetOneTask", &Master::GetOneTask, &master);
    server.register_handler("RegWorker", &Master::RegWorker, &master);
    server.register_handler("GetOneTask", &Master::GetOneTask, &master);
    server.register_handler("ReportTask", &Master::ReportTask, &master);
    // server.bind("ReportTask", &Master::ReportTask, &master);
    server.run();
    return 0;
}