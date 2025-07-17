cmake_minimum_required(VERSION 3.31)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads ON)

FetchContent_MakeAvailable(googletest)

add_executable(search_engine main.cpp
        ConverterJSON.cpp
        ConverterJSON.h
        InvertedIndex.cpp
        InvertedIndex.h
        SearchServer.cpp
        SearchServer.h
        tests.cpp)

target_include_directories(search_engine PRIVATE
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/json/include
)

target_link_libraries(search_engine PRIVATE gtest gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(search_engine)