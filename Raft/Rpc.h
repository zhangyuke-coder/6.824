#pragma once
#include <string>
#include "rest_rpc.hpp"
struct AppendEntriesArgs {
    int m_term;
    int m_leaderId;
    int m_prevLogIndex;
    int m_prevLogTerm;
    int m_leaderCommit;
    std::string m_sendLogs;
    MSGPACK_DEFINE(m_term, m_leaderId, m_prevLogIndex, m_prevLogTerm, m_leaderCommit, m_sendLogs);
};
struct AppendEntriesReply {
    int m_term;
    bool m_success;
    int m_conflict_term;        //用于冲突时日志快速匹配
    int m_conflict_index;       //用于冲突时日志快速匹配
    MSGPACK_DEFINE(m_term, m_success, m_conflict_index, m_conflict_index);
};

struct RequestVoteArgs {
    int term;
    int candidateId;
    int lastLogTerm;
    int lastLogIndex;
    MSGPACK_DEFINE(term, candidateId, lastLogTerm, lastLogIndex);
};

struct RequestVoteReply {
    int term;
    bool VoteGranted;
    MSGPACK_DEFINE(term, VoteGranted);
};