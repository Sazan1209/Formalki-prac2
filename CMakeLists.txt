cmake_minimum_required(VERSION 3.23)
project(formalki)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(sources
        src/Bitset.cpp
        src/NFA.cpp
        src/utility.cpp
        src/NFA.h
        src/Bitset.h
        src/globals.h
        src/utility.h
        Google_test/test1.cpp)

add_executable(formalki main.cpp ${sources})

add_subdirectory(Google_test)