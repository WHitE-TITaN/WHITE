#Set the minimum required version of CMake.
cmake_minimum_required(version 3.10)

#project name
project(KeepEYE)


#Set the C++ standard to C++17. PcapPlusPlus works well with modern C++.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

#Find PcapPlusPlus package Required for packet sniffing and manipulation.
find_package(PcapPlusPlus REQUIRED)

