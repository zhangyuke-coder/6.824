#pragma once 
#include "common.h"
#include "rpc.h"
#include <stdlib.h>
#include <cstdint>
#include <vector>
#include <mutex>
#include <deque>
#include <string>
enum class TaskStatus {
    TaskStatusReady   = 0,
    TaskStatusQueue   = 1,
    TaskStatusRunning = 2,
    TaskStatusFinish  = 3,
    TaskStatusErr     = 4
};

struct TaskStat {
    TaskStatus Status;
    int WorkerId;
    uint64_t StartTime;
};

class Master {
public:
    Master();
    ~Master();
    Task getTask(int taskSeq);

public:
    void reqTask(TaskArgs* args, Task* task);
    bool GetOneTask(TaskArgs* args, TaskReply* reply);
    bool ReportTask(ReportTaskArgs* args, ReportTaskReply* reply);
    bool RegWorker(RegisterArgs* args, RegisterReply* reply);
private:
    void schedule();
    void initMapTask();
    void initReduceTask();
    bool Done();
    void tickSchedule();
private:
    std::vector<std::string>        files_;
    int                             nReduce_;
    TaskPhase                       taskPhase_;
    std::vector<TaskStat>           taskStats_;
    std::mutex                      mtx_;
    bool                            done_;
    int                             workerSeq_;
    std::deque<Task>                tasks_;
};