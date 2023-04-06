#pragma once
#include <string>
#include "rest_rpc.hpp"

using TaskPhase = int;
constexpr TaskPhase MapPhase = 0;
constexpr TaskPhase ReducePhase = 1;

struct Task {
    std::string FileName;
    int NReduce;
    int NMaps;
    int Seq;
    TaskPhase Phase;
    bool Alive;
    MSGPACK_DEFINE(FileName, NReduce, NMaps, Seq, Phase, Alive);
};


