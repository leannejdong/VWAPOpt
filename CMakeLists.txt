cmake_minimum_required(VERSION 3.12)
project(VWAPOpt)

set(CMAKE_CXX_STANDARD 20)

add_executable(VWAPOpt main.cpp)

target_link_libraries(VWAPOpt boost_timer)