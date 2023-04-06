#include "worker.h"
#include <string>

#include "rpc.h"
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
    
    printf("worker get task \n");
    if(reply.task.Alive) {
        printf("Alive \n");
    }
    printf("reply %s \n", reply.task.FileName.c_str());
    printf("reply %d \n", reply.task.Seq);
    printf("%d \n", reply.task.Seq);
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

void Worker::doMapTask(Task t) {

    printf("--------------------doMapTask:%d---------------------------\n", t.Seq);
    std::cout << t.Seq << std::endl;
    std::cout << t.FileName << std::endl;
    std::cout << t.NMaps << std::endl;
    std::cout << t.NReduce << std::endl;
    printf("--------------------doMapTask:%d---------------------------\n", t.Seq); 
}


void Worker::doReduceTask(Task t) {
    printf("doReduceTask:%d\n", t.Seq);
}
