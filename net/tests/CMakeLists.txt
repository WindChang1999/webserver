set(TESTS
    EventLoopTest.cpp
    InetAddressTest.cpp
)

include(GoogleTest)
foreach(TEST IN LISTS TESTS)
    string(REGEX REPLACE ".cpp" "" TARGET ${TEST})
    add_executable(${TARGET} ${TEST})
    target_link_libraries(${TARGET} webserver_net GTest::gtest_main)
    gtest_discover_tests(${TARGET})
endforeach()