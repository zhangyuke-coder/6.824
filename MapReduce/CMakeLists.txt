# 这个是cmake最小版本要求
cmake_minimum_required(VERSION 3.1)

# Enable C++11，这一段是源码里的
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 填写你的项目名称Project
project( RPC )
include_directories(../)


# add_library(mapreduce master.cpp worker.cpp)
# target_link_libraries(mapreduce zmq)
# add_subdirectory(test)
add_executable(worker_main worker_main.cpp worker.cpp)
target_link_libraries(worker_main zmq pthread)
add_executable(master_main master_main.cpp master.cpp)
target_link_libraries(master_main zmq pthread)

add_executable(test test.cpp)
target_link_libraries(test pthread)
# add_executable(main_client main_client.cpp)
# target_link_libraries(main_client zmq)
# add_executable(main_server main_server.cpp)
# target_link_libraries(main_server zmq)