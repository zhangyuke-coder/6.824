#pragma once
#include "common.h"
struct TaskArgs {
    int WorkerId;
};

struct TaskReply
{
    Task* task;
};


struct ReportTaskArgs {
    bool Done;
    int Seq;
    TaskPhase Phase;
    int WorkerId;
};

struct ReportTaskReply {

};

struct RegisterArgs {

};

struct RegisterReply {
    int WorkerId;
};