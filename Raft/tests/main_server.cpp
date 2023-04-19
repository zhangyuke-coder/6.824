#include "rest_rpc.hpp"
using namespace rest_rpc;
using namespace rpc_service;
#include <fstream>

#include "../Rpc.h"
AppendEntriesReply test(rpc_conn conn, AppendEntriesArgs arg) {
  AppendEntriesReply reply;
  reply.m_success = false;
  reply.m_term = 1000;
  return reply;
}

int main() {
  //  benchmark_test();
  rpc_server server(9000, std::thread::hardware_concurrency());


  server.register_handler("test", test);
  server.run();

}