cmake_minimum_required(VERSION 3.10)
project(server_client)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE mysocket)