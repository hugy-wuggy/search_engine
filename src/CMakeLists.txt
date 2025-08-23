cmake_minimum_required(VERSION 3.31)

project(search_engine)

target_include_directories(search_engine
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/resources/
        ${CMAKE_SOURCE_DIR}/json/include/
)

configure_file(${CMAKE_SOURCE_DIR}/config/config.json ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/requests.json ${CMAKE_BINARY_DIR})

file(GLOB TXT_FILES "${CMAKE_SOURCE_DIR}/resources/*.txt")
file(COPY ${TXT_FILES} DESTINATION ${CMAKE_BINARY_DIR}/resources)
