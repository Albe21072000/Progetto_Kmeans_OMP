cmake_minimum_required(VERSION 3.23)
project(Kmeans)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp ")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static" )
add_executable(Kmeans cluster.cpp cluster.h)
add_executable(Kmeansseq Kmeansseq.cpp)
