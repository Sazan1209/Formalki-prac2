project(Google_test)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run test1.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)

