#pragma once
#include "common.h"
#include <rest_rpc.hpp>
struct TaskArgs {
    int WorkerId;
    MSGPACK_DEFINE(WorkerId)
};

struct TaskReply
{
    Task task;
    MSGPACK_DEFINE(task);
};


struct ReportTaskArgs {
    bool Done;
    int Seq;
    TaskPhase Phase;
    int WorkerId;
    MSGPACK_DEFINE(Done, Seq, Phase, WorkerId);
};

struct ReportTaskReply {
    MSGPACK_DEFINE();
};

struct RegisterArgs {
    MSGPACK_DEFINE();
};

struct RegisterReply {
    int WorkerId;
    MSGPACK_DEFINE(WorkerId);
};

struct person {
  int id;
  std::string name;
  int age;

  MSGPACK_DEFINE(id, name, age);
};
