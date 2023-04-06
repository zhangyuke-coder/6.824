#include <string>
#include "master.h"
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    // if(argc < 2){
    //     cout<<"missing parameter! The format is ./Master pg*.txt"<<endl;
    //     exit(-1);
    // }
    // alarm(10);

    rpc_server server(9000, std::thread::hardware_concurrency());
    vector<string> s{"asdf", "fasd"};
    Master master(s, 2);
    master.run();
    // server.bind("RegWorker", &Master::RegWorker, &master);
    // server.bind("GetOneTask", &Master::GetOneTask, &master);
    server.register_handler("RegWorker", &Master::RegWorker, &master);
    server.register_handler("GetOneTask", &Master::GetOneTask, &master);
    // server.bind("ReportTask", &Master::ReportTask, &master);
    server.run();
    return 0;
}