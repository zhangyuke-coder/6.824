add_executable(test test.cpp)
target_link_libraries(test )

add_executable(main_client main_client.cpp)
target_link_libraries(main_client pthread)
add_executable(main_server main_server.cpp)
target_link_libraries(main_server pthread)


add_executable(Time_test Time_test.cpp)
target_link_libraries(Time_test pthread)

add_executable(Channel_test Channel_test.cpp)
target_link_libraries(Channel_test pthread)