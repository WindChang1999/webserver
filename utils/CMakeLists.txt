file(GLOB UTILS_SRC *.cpp)

add_library(webserver_utils ${UTILS_SRC})

add_subdirectory(tests)