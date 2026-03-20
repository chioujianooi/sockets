cmake_minimum_required(VERSION 3.10)
project(Sockets)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sockets main.cpp  source/socket.h source/winsocket.cpp source/winsocket.h)

target_link_libraries(Sockets PRIVATE ws2_32)