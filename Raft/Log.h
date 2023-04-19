#pragma once
#include "Util.h"
#include <string>
#include <vector>

class Log {
public:
    void append(std::vector<Entry> entries) {
        for(auto e : entries) {
            Entries_.emplace_back(e);
        }
    }

    static Log makeEmptyLog() {
        auto log = Log();
        log.Index0_ = 0;
        return log;
    }

    Entry* at(int idx) {
        return &Entries_[idx];
    }
    void truncate(int idx) {
        std::vector<Entry> tmp;
        for(int i = 0; i < idx; i++) {
            tmp.emplace_back(Entries_[i]);
        }
        Entries_.clear();
        Entries_.swap(tmp);
    }

    std::vector<Entry> slice(int idx) {
        std::vector<Entry> res;
        for(int i = idx; i < Entries_.size(); i++) {
            res.emplace_back(Entries_[i]);
        }
        return res;
    }

    int len() {
        return Entries_.size();
    }

    Entry* lastLog() {
        return at(len() - 1);
    }

private:
    std::vector<Entry> Entries_;
    int Index0_;
};