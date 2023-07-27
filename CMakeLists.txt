cmake_minimum_required(VERSION 3.10)
project(httpserver VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
    -g
    -Wall
    -Wextra
    -Werror
)

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
include_directories(${PROJECT_SOURCE_DIR})
enable_testing()

add_subdirectory(net)
add_subdirectory(utils)
