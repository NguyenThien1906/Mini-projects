cmake_minimum_required(VERSION 3.5.0)
project(BigInt_test VERSION 0.1.0 LANGUAGES C CXX)

add_executable(loto main.cpp board.cpp board.hpp)

include(CTest)
enable_testing()

