cmake_minimum_required(VERSION 3.16.3)

project(parameter_pack_examples LANGUAGES CXX)

find_package(GTest REQUIRED)
include(GoogleTest)

include_directories(include)

add_executable ( mix_example examples/mix_example.cpp )
add_executable ( concat_example examples/concat_example.cpp )
add_executable ( typemanager_example examples/typemanager_example.cpp )
add_executable ( gtest_example examples/gtest_example.cpp )

target_link_libraries(gtest_example
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)