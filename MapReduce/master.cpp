#include "master.h"
#include <iostream>
#include <chrono>
#include <time.h>
static constexpr auto MaxTaskRunTime = std::chrono::seconds(5);
static constexpr auto ScheduleInterval = std::chrono::milliseconds(500);




Task Master::getTask(int taskSeq) {
    Task task;
    task.FileName = "";
    task.NReduce = nReduce_;
    task.NMaps = files_.size();
    task.Seq = taskSeq;
    task.Phase = taskPhase_;
    task.Alive = true;
}