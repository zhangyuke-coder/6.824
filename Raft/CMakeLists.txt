# 这个是cmake最小版本要求
cmake_minimum_required(VERSION 3.1)

# Enable C++11，这一段是源码里的
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 填写你的项目名称Project
project( Raft )
include_directories(../)


include_directories(
        "../rest_rpc/"
        "../rest_rpc/include"
        "../rest_rpc/thirdparty/msgpack-c/include"
        "../rest_rpc/thirdparty/asio")
add_subdirectory(tests)