add_executable(master_main master_main.cpp)
target_link_libraries(master_main mapreduce )
add_executable(worker_main worker_main.cpp)
target_link_libraries(worker_main mapreduce)