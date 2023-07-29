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
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG b796f7d44681514f58a683a3a71ff17c94edb0c1 
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG 7e635fca68d014934b4af8a1cf874f63989352b7
)
FetchContent_MakeAvailable(googletest spdlog)
include_directories(${PROJECT_SOURCE_DIR})
enable_testing()

add_subdirectory(net)
add_subdirectory(utils)
