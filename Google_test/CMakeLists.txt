project(Google_test)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run ${sources} test1.cpp test2.cpp testing_funcs.h testing_uncs.cpp)
target_compile_definitions(Google_Tests_run PRIVATE TEST_DIR=\"${PROJECT_SOURCE_DIR}\")

target_link_libraries(Google_Tests_run gtest gtest_main)

