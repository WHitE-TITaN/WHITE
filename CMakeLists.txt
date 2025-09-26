#Set the minimum required version of CMake.
cmake_minimum_required(VERSION 3.10)

#project name
project(WHITE)


#Set the C++ standard to C++17. PcapPlusPlus works well with modern C++.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add include directories
include_directories(${CMAKE_SOURCE_DIR}/SandBox/headers)

add_executable(
  WHITE
  src/WHITE.cpp
)