#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include "wc.hpp"
using namespace std;
std::string readAllFile(std::string filename) {
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    return str;
}
int main() {

    //获取文件名称, 过滤出txt文件
    std::string dir = "/home/zyk/VcWorkplace/6.824/MapReduce";
    DIR* d = opendir(dir.c_str());
    if (d == NULL)
    {
            printf("d == NULL");
    }
    std::vector<std::string> targets;
    struct dirent* entry;
    while ( (entry=readdir(d)) != NULL)
    {
            std::string name = entry->d_name;
            if(name.substr(0, 2) == "pg") {
                targets.push_back(dir+ '/' + name);
            }

    }
    std::vector<KeyValue> intermediate;
    intermediate.reserve(700000);
    for(auto s : targets) {
        string target_s = readAllFile(s);
        auto targetMap = func::Map("", target_s);
        for(auto kv : targetMap) {
            intermediate.push_back(kv);
        }
    }
    sort(intermediate.begin(), intermediate.end(), [](KeyValue a, KeyValue b){ return a.Key < b.Key; });

    cout << intermediate.size() << endl;
    cout << intermediate.back().Key << " " << intermediate.back().Value << endl;
    
    // string s = readAllFile("/home/zyk/VcWorkplace/6.824/MapReduce/zyk.txt");
    // auto res = func::Map("zyk.txt", s);
    // for(auto r : res) {
    //     cout << r.Key << "   " << r.Value << endl;
    // }
    
}