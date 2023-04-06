#pragma once
#include <string>
#include <functional>
#include <vector>
#include "common.h"
#include "rpc.h"

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
    
    bool reqTask(Task* t);
    void doTask(Task t);
    void doMapTask(Task t);
    void doReduceTask(Task t);
    void reportTask(Task t, bool done, bool err);
    void wregister();
    bool call(std::string rpcname, void* args, void* reply);
private:
    rest_rpc::rpc_client*   work_client_;
    int         id_;
    MapFunc     mapf_;
    ReduceFunc  reducef_;
    int         maxCount_;
};