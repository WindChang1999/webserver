set(NET_SRC
    Channel.cpp
    EventLoop.cpp
    Poller.cpp
    InetAddress.cpp
)

add_library(webserver_net ${NET_SRC})
target_link_libraries(webserver_net PRIVATE webserver_utils)
target_link_libraries(webserver_net PRIVATE spdlog::spdlog)

add_subdirectory(tests)