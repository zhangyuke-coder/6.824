#include "worker.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <ios>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "asio/error.hpp"
#include "rpc.h"
#include "wc.hpp"
static std::string readAllFile(std::string filename) {
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    return str;
}
Worker::Worker() {
    maxCount_ = 1000000;
    work_client_ = new rest_rpc::rpc_client("127.0.0.1", 9000);
    bool r = work_client_->connect();
    // work_client_->enable_auto_heartbeat();
    if (!r) {
        std::cout << "connect timeout" << std::endl;
    }
    wregister();
    run();
}
Worker::~Worker() {
    work_client_->close();
    delete work_client_;
}
void Worker::wregister(){
    RegisterArgs args;
    auto reply = work_client_->call<RegisterReply>("RegWorker", args);
    // printf("%d \n", reply.val().WorkerId);
    printf("request worker id success: %d\n", reply.WorkerId);
    id_ = reply.WorkerId;
}

void Worker::run() {
    int count = 0;
    while(1) {
        
        Task t;
        bool flag = reqTask(&t);
        if(!flag) {
            if(count++ < maxCount_) {
                continue;
            } else {
                return;
            }

        }
        if(!t.Alive) {
            printf("worker get task not alive, exit\n");
            // return;
            return;
        }
        doTask(t);
    }
}

bool Worker::reqTask(Task* t) {
    TaskArgs args;
    args.WorkerId = id_;
    TaskReply reply;
    try {
        reply = work_client_->call<TaskReply>("GetOneTask", args);
    } catch (std::exception e) {
        return false;
    }
    *t = reply.task;
    return true;
}

void Worker::doTask(Task t) {
    printf("in do Task\n");
    switch (t.Phase)
    {
    case MapPhase:
        doMapTask(t);
        break;
    case ReducePhase:
        doReduceTask(t);
        break;
    
    default:
        break;
    }
}
std::string reduceName(int mapIdx, int reduceIdx) {
    return "mr-" + std::to_string(mapIdx) + "-" + std::to_string(reduceIdx);
}
void Worker::doMapTask(Task t) { 

    std::string target_s = readAllFile(t.FileName);
    auto targetMap = func::Map("", target_s);
    std::vector<std::vector<KeyValue>> reduces(t.NReduce);
    for(auto kv : targetMap) {
        auto idx = ihash(kv.Key) % t.NReduce;
        reduces[idx].push_back(kv);
    }

    for(int i = 0; i < reduces.size(); i++) {
        auto fileName = reduceName(t.Seq, i);
        std::ofstream ofs;
        ofs.open(fileName, std::ios::app);
        std::sort(reduces[i].begin(), reduces[i].end(), [](KeyValue a, KeyValue b){ return a.Key < b.Key; });
        for(auto r : reduces[i]) {
            ofs << r.Key << " " << r.Value << std::endl;
        }
        ofs.close();
        reportTask(t, true, true);
    }
    printf("--------------------doMapTask Done:%d---------------------------\n", t.Seq); 
}

std::vector<std::string> split(std::string& strs) {
    std::vector<std::string> vec;
    std::string str;
    for(auto c : strs) {
        if(c != ' ' && c != '\n') {
            str += c;
        } else {
            vec.push_back(str);
            str = "";
        }
    }
    vec.push_back(str);
    return vec;
}
void Worker::doReduceTask(Task t) {
    printf("doReduceTask:%d\n", t.Seq);
    // for(int i = 0; i < t.NMaps; i++) {
    //     auto fileName = reduceName(i, t.Seq);
    //     auto contents = readAllFile(fileName);

    // }
    std::unordered_map<std::string, std::vector<std::string>> maps;
    std::string res;
    for(int i = 0;i < t.NMaps; i++) {
        std::string filename = reduceName(i, t.Seq);
        printf("filename: %s\n", filename.c_str());
        std::string query;
        std::ifstream in(filename);
        while(getline(in,query)){
            auto vec = split(query);
            // std::cout << query << std::endl;
            // std::cout << vec[0] << " " << vec[1] << std::endl;
            maps[vec[0]].push_back(vec[1]);
        }
 
        for(auto map : maps) {
            auto temp = func::Reduce(map.first, map.second);
            // std::cout << temp << std::endl;
            res = res + map.first + ' ' + temp + '\n';
        }

        // std::cout << res << std::endl;
    }
    std::string reduceName = "mr-out-" + std::to_string(t.Seq);
    std::cout << reduceName << std::endl;
    std::ofstream ofs;
    ofs.open(reduceName, std::ios::app);
    ofs << res;
    ofs.close();
    reportTask(t, true, true);
    printf("doReduceTask--------------------done:%d\n", t.Seq);
}

void Worker::reportTask(Task t, bool done, bool err) {
    ReportTaskArgs args;
    args.Done = done;
    args.Seq = t.Seq;
    args.Phase = t.Phase;
    args.WorkerId = id_;
    ReportTaskReply reply;
  
        reply = work_client_->call<ReportTaskReply>("ReportTask", args);
   
    
    
}
