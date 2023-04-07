#include "master.h"
#include <iostream>
#include <chrono>
#include <mutex>
#include <time.h>

static constexpr auto MaxTaskRunTime = std::chrono::seconds(5);
static constexpr auto ScheduleInterval = std::chrono::milliseconds(500);


Master::Master(std::vector<std::string> files, int nReduce)
    : files_(files), nReduce_(nReduce), workerSeq_(0), done_(false) {
    // if(nReduce_ > files.size()) {
    //     tasks_.resize(nReduce);
    // } else {
    //     tasks_.resize(files.size());
    // }
    initMapTask();
    // std::thread(tickSchedule, this);
}
Master::~Master() {
    t_.join();
}
Task Master::getTask(int taskSeq) {
    Task task;
    task.FileName = "";
    task.NReduce = nReduce_;
    task.NMaps = files_.size();
    task.Seq = taskSeq;
    task.Phase = taskPhase_;
    task.Alive = true;
    if(task.Phase == MapPhase) {
        task.FileName = files_[taskSeq];
    }
    return task;
}

void Master::initMapTask() {
    taskPhase_ = MapPhase;
    taskStats_.resize(files_.size());
    for(auto task : taskStats_) {
        task.Status = TaskStatus::TaskStatusReady;
    }
}

void Master::initReduceTask() {
    taskPhase_ = ReducePhase;
    taskStats_.clear();
    taskStats_.resize(nReduce_);
    for(auto task : taskStats_) {
        task.Status = TaskStatus::TaskStatusReady;
    }
}
RegisterReply Master::RegWorker(rpc_conn conn, RegisterArgs args) {

    RegisterReply reply;
    reply.WorkerId = workerSeq_;
    workerSeq_++;
    return reply;
}
ReportTaskReply Master::ReportTask(rpc_conn conn, ReportTaskArgs args) {
    std::lock_guard<std::mutex> lk(mtx_);
    printf("get report task workid%d:, get report task seq:%d\n", args.WorkerId, args.Seq);
    printf("get this workid:%d\n", taskStats_[args.Seq].WorkerId);
    if(args.Phase != taskPhase_ || args.WorkerId != taskStats_[args.Seq].WorkerId) {

        return {};
    }
    if(args.Done) {

        taskStats_[args.Seq].Status = TaskStatus::TaskStatusFinish;
       
    } else {
        taskStats_[args.Seq].Status = TaskStatus::TaskStatusErr;
    }

    // schedule();
    //                          printf("----------ReportTask---------------------\n");
    return {};
}
void Master::run() {
    t_ = std::thread(&Master::tickSchedule, this);
    // tickSchedule();
}
void Master::schedule() {
    if(done_) {
        return;
    }
    std::unique_lock<std::mutex> lk(mtx_);
    bool allfinish = true;
    for(int i = 0; i < taskStats_.size(); i++) {
        switch (taskStats_[i].Status)
        {
        case TaskStatus::TaskStatusReady:
            allfinish = false;
            {
                
                auto t = getTask(i);
                tasks_.push_back(t);
                taskStats_[i].Status = TaskStatus::TaskStatusQueue;
                cv_.notify_one();
            }
            break;
        case TaskStatus::TaskStatusQueue:
            allfinish = false;
            break;
        case TaskStatus::TaskStatusRunning:
            allfinish = false;
            if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - taskStats_[i].StartTime) > MaxTaskRunTime) {
                taskStats_[i].Status = TaskStatus::TaskStatusQueue;
                {
                    auto t = getTask(i);
                    tasks_.push_back(t);
                    cv_.notify_one();
                }
            }
            break;
        case TaskStatus::TaskStatusFinish:
            /* code */
            break;
        case TaskStatus::TaskStatusErr:
            allfinish = false;
            taskStats_[i].Status = TaskStatus::TaskStatusQueue;
            {

                auto t = getTask(i);
                tasks_.push_back(t);
                cv_.notify_one();
            }
            break;
        
        default:
            break;
        }

    }


    if(allfinish) {
        if(taskPhase_ == MapPhase) {
            initReduceTask();
        } else {
            done_ = true;
        }
    }
}
void Master::tickSchedule() {
    while(!done_) {
        schedule();
        std::this_thread::sleep_for(ScheduleInterval);
    }
}

TaskReply Master::GetOneTask(rpc_conn conn, TaskArgs args) {
    std::unique_lock<std::mutex> lk(mtx_);
    while(tasks_.empty()) {
        cv_.wait(lk);
    }
    auto task = tasks_.front();
    tasks_.pop_front();
    TaskReply reply;
    
    reply.task = task;
    // printf("in get one Task, \n");
    // printf("GetOneTask------------------------------------\n");
    // if(reply.task.Alive) {
    //     printf("Alive \n");
    // }
    // printf("reply %s \n", reply.task.FileName.c_str());
    // printf("reply %d \n", reply.task.Seq);
    // printf("GetOneTask------------------------------------\n");
    return reply;
}

void Master::regTask(TaskArgs* args, Task* task) {
    std::lock_guard<std::mutex> lk(mtx_);
    if(task->Phase != taskPhase_) {
        printf("req task phase neq\n");
    }
    taskStats_[task->Seq].Status = TaskStatus::TaskStatusRunning;
    taskStats_[task->Seq].WorkerId = args->WorkerId;
    taskStats_[task->Seq].StartTime = std::chrono::system_clock::now();
}