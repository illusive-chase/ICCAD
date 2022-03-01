cmake_minimum_required(VERSION 3.18)
project(ICCAD)
set(SRC ./src/IC_Graph.cc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(./src)

add_library(lib ${SRC})
add_executable(MNA ./src/exe/MNA.cc)

target_link_libraries(MNA lib)