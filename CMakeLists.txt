cmake_minimum_required(VERSION 3.12)
project(VWAPOpt)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED COMPONENTS timer)
add_executable(VWAPOpt main.cpp)

target_link_libraries(VWAPOpt PRIVATE Boost::timer)