#include "Raft.h"
#include "Raft/Rpc.h"
#include "Raft/Util.h"
#include <chrono>
#include <ctime>
#include <functional>
#include <mutex>
#include <thread>


void Raft::Make(std::vector<PeersInfo> &peers, int me, 
        Persister *persister, go::Chan<ApplyMsg>* applych) {
    peers_ = peers;
    persister_ = persister;
    me_ = me;

    state_ = RaftState::FOLLOWER;
    currentTerm_ = 0;
    votedFor_ = -1;
    heartBeat_ = 50;

    log_ = Log::makeEmptyLog();
    log_.append({Entry{"", 0, 0}});
    commitIndex_ = 0;
    lastApplied_ = 0;
    nextIndex_.resize(peers_.size());
    matchIndex_.resize(peers_.size());
    becomeLeader_ = false;
    applych_ = applych;

    std::thread t1(&Raft::listenForVote);
    t1.detach();

    std::thread t2(&Raft::ticker, this);
    t2.detach();
    electionTime_.start(200, std::bind(&Raft::leaderElection, this));
    // std::thread t1(&Raft::)

}
void Raft::setNewTerm(int term) {
    if(term > currentTerm_ || currentTerm_ == 0) {
        state_ = RaftState::FOLLOWER;
        currentTerm_ = term;
        votedFor_ = -1;

    }
}
void Raft::resetElectionTimer() {
    electionTime_.stop();
    electionTime_.start(200, std::bind(&Raft::leaderElection, this));
}

void Raft::leaderElection() {
    electionTime_.stop();
    becomeLeader_ = false;
    currentTerm_++;
    state_ = RaftState::CANDIDATE;
    votedFor_ = me_;
    
    int term = currentTerm_;
    int voteCounter = 1;
    auto lastLog = log_.lastLog();

    RequestVoteArgs args;
    args.term = term;
    args.candidateId = me_;
    args.lastLogTerm = lastLog->Term_;
    args.lastLogIndex = lastLog->Index_;

    for(int serverId = 0; serverId < peers_.size(); serverId++) {
        if(serverId != me_) {
            std::thread(&Raft::candidateRequestVote, this, serverId, &args, &voteCounter).detach();
        }
    }

    if(state_ != RaftState::LEADER) {
        electionTime_.start(200, std::bind(&Raft::leaderElection, this));
    }
}

void Raft::ticker() {
    while(!killed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(heartBeat_));
        std::lock_guard<std::mutex> lk(mu_);
        if(state_ == RaftState::LEADER) {

        }

    }
}

void Raft::candidateRequestVote(int serverId, RequestVoteArgs *args, int *voteCounter) {
    RequestVoteReply reply;
    auto ok = sendRequestVote(serverId, args, &reply);
    if(!ok) return;
    std::lock_guard<std::mutex> lk(mu_);
    if(reply.term > args->term) {
        setNewTerm(reply.term);
        return;
    }

    if(reply.term < args->term) {
        return;
    }
    if(!reply.VoteGranted) {
        return;
    }
    (*voteCounter)++;
    if((*voteCounter) > peers_.size() / 2 &&
        currentTerm_ == args->term &&
        state_ == RaftState::CANDIDATE) {
        if(!becomeLeader_) {
            becomeLeader_ = true;
            state_ = RaftState::LEADER;
            auto lastLogIndex = log_.lastLog()->Index_;
            for(int i = 0; i < peers_.size(); i++) {
                nextIndex_[i] = lastLogIndex + 1;
                matchIndex_[i] = 0;
            }
            
        } 
    }

}

void Raft::listenForVote(void* arg){
    Raft* raft = (Raft*)arg;
    rpc_server server(raft->peers_[raft->me_].electionPort, std::thread::hardware_concurrency());


    server.register_handler("Raft.RequestVote", &Raft::RequestVote, raft);

    server.run();
    // printf("exit!\n");
}

RequestVoteReply Raft::RequestVote(RequestVoteArgs args) {
    RequestVoteReply reply;
    std::lock_guard<std::mutex> lk(mu_);
    if(args.term < currentTerm_) {
        reply.VoteGranted = false;
        return reply;
    }
    auto myLastLog = log_.lastLog();
    bool upToDate = args.lastLogTerm > myLastLog->Term_ || 
        (args.lastLogTerm == myLastLog->Term_ && args.lastLogIndex >= myLastLog->Index_);
    if((votedFor_ == -1 || votedFor_ == args.candidateId) && upToDate) {
        reply.VoteGranted = true;
        votedFor_ = args.candidateId;
        resetElectionTimer();

    } else {
        reply.VoteGranted = false;
    }
    reply.term = currentTerm_;
    return reply;
}

bool Raft::sendRequestVote(int server, RequestVoteArgs* args, RequestVoteReply* reply) {

    rest_rpc::rpc_client client("127.0.0.1", peers_[server].electionPort);
    bool r = client.connect();
    if (!r) {
        std::cout << "connect timeout" << std::endl;
        return false;
    }
    *reply = client.call<RequestVoteReply>("Raft.RequestVote", args);
    return true;
}