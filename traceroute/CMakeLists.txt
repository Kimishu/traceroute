cmake_minimum_required(VERSION 3.24)
project(traceroute)

set(CMAKE_CXX_STANDARD 14)

add_executable(traceroute
        src/main.cpp src/Tracerouter/Tracerouter.h src/Tracerouter/Tracerouter.cpp)

target_link_libraries(traceroute PRIVATE Ws2_32)
