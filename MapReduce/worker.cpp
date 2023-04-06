#include "worker.h"
#include <string>

#include "rpc.h"
Worker::Worker() {
    buttonrpc work_client;
    work_client.as_client("127.0.0.1", 5555);
    work_client.set_timeout(5000);  
    work_client_ = &work_client;
    wregister();
    run();
}
Worker::~Worker() {
    work_client_->~buttonrpc();
    delete work_client_;
}
void Worker::wregister(){
    RegisterArgs args;
    auto reply = work_client_->call<RegisterReply>("RegWorker", args);
    // printf("%d \n", reply.val().WorkerId);
    printf("request worker id success: %d\n", reply.val().WorkerId);
    id_ = reply.val().WorkerId;
}

void Worker::run() {
    while(1) {
        auto t = reqTask();
        if(!t.Alive) {
            printf("worker get task not alive, exit\n");
            // return;
            return;
        }
        doTask(t);
    }
    
}

Task Worker::reqTask() {
    TaskArgs args;
    args.WorkerId = id_;
    auto reply = work_client_->call<TaskReply>("GetOneTask", args);
    if(!reply.valid()) {
        // printf("%s", reply.error_msg());
    }
    printf("worker get task \n");
    return reply.val().task;
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

    printf("doMapTask:%d\n", t.Seq);
}


void Worker::doReduceTask(Task t) {
    printf("doReduceTask:%d\n", t.Seq);
}
