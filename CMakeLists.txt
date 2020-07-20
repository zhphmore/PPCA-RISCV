cmake_minimum_required(VERSION 3.10)
project(PPCARISCV)

set(CMAKE_CXX_STANDARD 11)

add_executable(PPCARISCV main.cpp theriscv.h pipelineIF.h pipelineID.h pipelineEX.h pipelineMEM.h pipelineWB.h)
