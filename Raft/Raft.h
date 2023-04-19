#pragma once
#include "ChannelsCPP/ChannelsCPP/ChannelUtility.h"
#include "Timer.hpp"
#include "rest_rpc.hpp"
#include "Rpc.h"
#include "Log.h"
#include "Util.h"
#include "ChannelsCPP/ChannelsCPP/Channel.h"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <string>
#include <condition_variable>
#include <vector>

class Raft {
public:
    static void listenForVote(void* arg);
public:
    void Make(std::vector<PeersInfo>& peers, int me, Persister* persister = nullptr, 
                go::Chan<ApplyMsg>* applych = nullptr);
    void resetElectionTimer();           
    void leaderElection();
    void candidateRequestVote(int serverId, RequestVoteArgs* args, int* voteCounter);
    bool sendRequestVote(int server, RequestVoteArgs* args, RequestVoteReply* reply);
    RequestVoteReply RequestVote(RequestVoteArgs args);
    void ticker();
    void setNewTerm(int term);
    void Kill() {   
        dead_ = 1;
    }
    bool killed() {
        return dead_ == 1;
    }
private:
    std::mutex mu_;
    // std::condition_variable m_cond;
    std::vector<PeersInfo> peers_;
    Persister* persister_;
    int me_;

    std::atomic<int32_t> dead_;
    
    RaftState state_;
    go::Chan<Entry>* appendEntryCh_;
    int heartBeat_;
    Timer electionTime_;

    int currentTerm_;
    int votedFor_;
    Log log_;

    int commitIndex_;
    int lastApplied_;

    std::vector<int> nextIndex_;
    std::vector<int> matchIndex_;

    go::Chan<ApplyMsg>* applych_;
    std::condition_variable applyCond_;

    std::atomic<bool> becomeLeader_;
};