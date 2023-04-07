#include "worker.h"
#include <algorithm>
#include <ios>
#include <string>
#include <fstream>
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
    maxCount_ = 10;
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
        // printf("------------------run------------------\n");
        // printf("worker get task \n");
        // if(t.Alive) {
        //     printf("Alive \n");
        // }
        // printf("reply %s \n", t.FileName.c_str());
        // printf("reply %d \n", t.Seq);
        // printf("------------------run------------------\n");

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

    printf("--------------------doMapTask:%d---------------------------\n", t.Seq);
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
    printf("--------------------doMapTask:%d---------------------------\n", t.Seq); 
}


void Worker::doReduceTask(Task t) {
    printf("doReduceTask:%d\n", t.Seq);
    // for(int i = 0; i < t.NMaps; i++) {
    //     auto fileName = reduceName(i, t.Seq);
    //     auto contents = readAllFile(fileName);

    // }
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
