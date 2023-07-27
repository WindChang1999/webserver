add_executable(TimeStampTest TimeStampTest.cpp)
target_link_libraries(TimeStampTest webserver_utils GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TimeStampTest)