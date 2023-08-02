#include<gtest/gtest.h>
#include<string>
#include<thread>
#include<net/EventLoop.hpp>
#include<sys/timerfd.h>
#include<net/Channel.hpp>

TEST(EventLoopTest, TriggerTimerOnce) {
    EventLoop loop;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    Channel ch(&loop, timerfd);

    ch.setReadCallBack([&loop] { loop.quit(); });
    ch.enableReading();

    struct itimerspec timeout;
    bzero(&timeout, sizeof(timeout));
    timeout.it_value.tv_sec = 1;
    ::timerfd_settime(timerfd, 0, &timeout, nullptr);
    
    loop.loop();
    ::close(timerfd);
}