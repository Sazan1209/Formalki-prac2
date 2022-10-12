cmake_minimum_required(VERSION 3.23)
project(formalki)

set(CMAKE_CXX_STANDARD 17)

add_executable(formalki main.cpp NFA.h Bitset.cpp Bitset.h globals.h NFA.cpp utility.h utility.cpp)
