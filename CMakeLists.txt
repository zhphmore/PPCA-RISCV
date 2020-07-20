cmake_minimum_required(VERSION 3.10)
project(PPCARISCV)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++11 -g")

add_executable(PPCARISCV main.cpp theriscv.hpp pipelineIF.hpp pipelineID.hpp pipelineEX.hpp pipelineMEM.hpp pipelineWB.hpp)
