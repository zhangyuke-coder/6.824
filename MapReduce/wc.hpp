#pragma once
#include <cctype>
#include <vector>
#include <string>
#include "worker.h"
namespace func {
std::vector<KeyValue> Map(std::string filename, std::string contents) {
    std::vector<KeyValue> res;
    std::string tmp;
    for(auto c : contents) {
        if(std::isalpha(c)) {
            tmp += c;
        } else {
            if(!tmp.empty()) res.push_back({tmp, "1"});
            tmp = "";
        }
    }
    if(tmp.size() != 0) res.push_back({tmp, "1"});
    return res;
}


std::string Reduce(std::string key, std::vector<std::string> values) {
    return std::to_string(values.size());
}
};