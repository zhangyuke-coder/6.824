#include <chrono>
#include <fstream>
#include <iostream>
#include "rest_rpc.hpp"
#include "../Rpc.h"
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;


int main() {
  rpc_client client("127.0.0.1", 9000);
  bool r = client.connect();
  if (!r) {
    std::cout << "connect timeout" << std::endl;
    return 1;
  }
  AppendEntriesArgs args;
  auto result = client.call<AppendEntriesReply>("test", args);
  std::cout << result.m_term << std::endl;
  // auto reply = client.call<RegisterReply>("reqwork");
  // std::cout << reply.WorkerId << std::endl;
  return 0;
}