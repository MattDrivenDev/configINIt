cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(configINIt)

add_library(configINIt INTERFACE)

target_include_directories(configINIt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(configINIt_test
    test/main.cpp
)

target_link_libraries(configINIt_test PRIVATE configINIt)

configure_file(test/config.ini config.ini COPYONLY)