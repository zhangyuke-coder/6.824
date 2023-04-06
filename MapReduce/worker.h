#pragma once
#include <string>
#include <functional>
#include <vector>
#include "common.h"
#include "buttonrpc/buttonrpc.hpp"

struct KeyValue
{
    std::string Key;
    std::string Value;
};
typedef std::vector<KeyValue> (*MapFunc)(std::string, std::string);
typedef std::string (*ReduceFunc)(std::string, std::vector<std::string>);



static int ihash(std::string str){
    int sum = 0;
    for(int i = 0; i < str.size(); i++){
        sum += (str[i] - '0');
    }
    return sum;
}

class Worker {
public:

    Worker();
    ~Worker();
    void run();
    
    Task reqTask();
    void doTask(Task t);
    void doMapTask(Task t);
    void doReduceTask(Task t);
    void reportTask(Task t, bool done, bool err);
    void wregister();
    bool call(std::string rpcname, void* args, void* reply);
private:
    buttonrpc*   work_client_;
    int         id_;
    MapFunc     mapf_;
    ReduceFunc  reducef_;
};