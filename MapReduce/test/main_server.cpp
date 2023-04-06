#include <rest_rpc.hpp>
using namespace rest_rpc;
using namespace rpc_service;
#include <fstream>

#include "rpc.h"



person get_person(rpc_conn conn) { return {1, "tom", 20}; }

RegisterReply reqwork(rpc_conn conn) {
  RegisterReply reply;
  reply.WorkerId = 4;
  return reply;
}
int main() {
  //  benchmark_test();
  rpc_server server(9000, std::thread::hardware_concurrency());


  server.register_handler("get_person", get_person);
  server.register_handler("reqwork", reqwork);
  server.run();

}