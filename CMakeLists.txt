cmake_minimum_required(VERSION 3.23)
project(formalki)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

set(sources
        ${PROJECT_SOURCE_DIR}/src/Bitset.cpp
        ${PROJECT_SOURCE_DIR}/src/NFA.cpp
        ${PROJECT_SOURCE_DIR}/src/utility.cpp
        ${PROJECT_SOURCE_DIR}/src/NFA.h
        ${PROJECT_SOURCE_DIR}/src/Bitset.h
        ${PROJECT_SOURCE_DIR}/src/globals.h
        ${PROJECT_SOURCE_DIR}/src/utility.h
        ${PROJECT_SOURCE_DIR}/src/DFA.cpp
        ${PROJECT_SOURCE_DIR}/src/DFA.h)

add_executable(formalki main.cpp ${sources})

add_subdirectory(Google_test)