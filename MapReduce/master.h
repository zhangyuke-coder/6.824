#pragma once 
#include "common.h"
#include "rpc.h"
#include <stdlib.h>
#include <cstdint>
#include <vector>
#include <mutex>
#include <deque>
#include <atomic>
#include <condition_variable>
#include <string>
#include <thread>
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
    std::chrono::system_clock::time_point StartTime;
};

class Master {
public:
    Master(std::vector<std::string> files, int nReduce);
    ~Master();
    Task getTask(int taskSeq);
    void run();
public:
    void regTask(TaskArgs* args, Task* task);
    TaskReply GetOneTask(TaskArgs args);
    ReportTaskReply ReportTask(ReportTaskArgs* args);
    RegisterReply RegWorker(RegisterArgs* args);
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
    std::condition_variable         cv_;
    std::atomic<bool>               done_;
    std::atomic<int>                workerSeq_;
    std::deque<Task>                tasks_;
    std::thread                     t_;
};