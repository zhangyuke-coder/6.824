# 这个是cmake最小版本要求
cmake_minimum_required(VERSION 3.1)

# Enable C++11，这一段是源码里的
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 填写你的项目名称Project
project( RPC )
include_directories(../)
add_executable(main_client main_client.cpp)
target_link_libraries(main_client zmq)
add_executable(main_server main_server.cpp)
target_link_libraries(main_server zmq)