#include "buttonrpc/buttonrpc.hpp"
#include "master.h"
#include <iostream>

int main(int argc, char* argv[]){
    // if(argc < 2){
    //     cout<<"missing parameter! The format is ./Master pg*.txt"<<endl;
    //     exit(-1);
    // }
    // alarm(10);
    buttonrpc server;
    server.as_server(5555);
    vector<string> s{"asdf", "fasd"};
    Master master(s, 2);
    master.run();
    server.bind("RegWorker", &Master::RegWorker, &master);
    server.bind("GetOneTask", &Master::GetOneTask, &master);
    // server.bind("ReportTask", &Master::ReportTask, &master);
    server.run();
    return 0;
}