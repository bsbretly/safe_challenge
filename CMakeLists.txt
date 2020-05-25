cmake_minimum_required(VERSION 3.11)
project(safe_challenge)

set(CMAKE_CXX_STANDARD 11)

set(sources main.cpp crack_safe.cpp include/crack_safe.h include/helpers.h helpers.cpp)
include_directories(include)
add_executable(safe_challenge ${sources})