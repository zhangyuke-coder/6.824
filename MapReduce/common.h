#pragma once
#include <string>


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
};


