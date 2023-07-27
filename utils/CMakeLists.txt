set(UTILS_SRC
    TimeStamp.cpp
)

add_library(webserver_utils ${UTILS_SRC})

add_subdirectory(tests)