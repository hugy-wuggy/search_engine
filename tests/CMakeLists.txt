cmake_minimum_required(VERSION 3.8)

project(server_test)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(server_test
        ${CMAKE_SOURCE_DIR}/tests/main.cpp
        ${CMAKE_SOURCE_DIR}/tests/index_tests.cpp
        ${CMAKE_SOURCE_DIR}/tests/server_tests.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
        ${CMAKE_SOURCE_DIR}/src/ConverterJSON.cpp
)

target_include_directories(server_test PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/json/include/
)

target_link_libraries(server_test PRIVATE gtest_main)

add_test(NAME server_test COMMAND server_test)