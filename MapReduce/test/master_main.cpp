#include "buttonrpc/buttonrpc.hpp"
#include "../master.h"
#include <iostream>

int main(int argc, char* argv[]){
    if(argc < 2){
        cout<<"missing parameter! The format is ./Master pg*.txt"<<endl;
        exit(-1);
    }
    // alarm(10);
    buttonrpc server;
    server.as_server(5555);
    vector<string> s{"asdf", "fasd"};
    Master master(s, 9);
    // master.GetAllFile(argv, argc);
    // server.bind("getMapNum", &Master::getMapNum, &master);
    // server.bind("getReduceNum", &Master::getReduceNum, &master);
    // server.bind("assignTask", &Master::assignTask, &master);
    // server.bind("setMapStat", &Master::setMapStat, &master);
    // server.bind("isMapDone", &Master::isMapDone, &master);
    // server.bind("assignReduceTask", &Master::assignReduceTask, &master);
    // server.bind("setReduceStat", &Master::setReduceStat, &master);
    // server.bind("Done", &Master::Done, &master);
    server.bind("RegWorker", &Master::RegWorker);
    server.run();
    return 0;
}