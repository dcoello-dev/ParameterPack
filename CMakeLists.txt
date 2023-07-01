cmake_minimum_required(VERSION 3.16.3)

project(parameter_pack_examples LANGUAGES CXX)

find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(include)

file(GLOB_RECURSE TEST_SRCS
    ./test/*.cpp
)

add_executable(tests ${TEST_SRCS})

target_link_libraries(tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)
