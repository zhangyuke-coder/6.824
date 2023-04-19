#pragma once
#include <chrono>
#include <thread>
#include <utility>
#include <vector>
struct PeersInfo {
    int logPort;
    int electionPort;
    int m_peerId;
};

struct Entry {
    std::string Command_;
    int Term_;
    int Index_;
};
enum class RaftState {
    LEADER = 0,
    CANDIDATE,
    FOLLOWER
};

class Persister{
public:
    std::vector<Entry> logs;
    int cur_term;
    int votedFor;
};
struct ApplyMsg {
    bool CommandValid;
    std::string Command;
    int CommandIndex;

	// // For 2D:
	// SnapshotValid bool
	// Snapshot      []byte
	// SnapshotTerm  int
	// SnapshotIndex int
};
const auto ElectionTimeout = std::chrono::milliseconds(300);
const auto HeartBeatTimeout = std::chrono::milliseconds(150);
const auto ApplyInterval = std::chrono::milliseconds(100);
const auto RPCTimeout = std::chrono::milliseconds(100);
const auto MaxLockTime = std::chrono::milliseconds(10);