#include<gtest/gtest.h>
#include<string>
#include<thread>
#include<net/EventLoop.hpp>

TEST(EventLoopTest, StartEventLoop) {
    EventLoop loop;
    loop.loop();
}

TEST(EventLoopTest, StartEventLoopTwice) {
    EventLoop loop;
    loop.loop();
    EXPECT_DEATH(loop.loop(), "");
}