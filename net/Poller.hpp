#ifndef NET_POLLER_HPP
#define NET_POLLER_HPP

#include<vector>
#include<map>
#include<utils/TimeStamp.hpp>

struct pollfd;
class Channel;
class EventLoop;
class TimeStamp;

class Poller {
public:
    using ChannelList = std::vector<Channel*>;
private:
    using PollFdList = std::vector<struct pollfd>;
    using ChannelMap = std::map<int, Channel*>;

    EventLoop* _ownerLoop;
    PollFdList _pollfds;
    ChannelMap _channels;
public:
    Poller(EventLoop* loop);
    ~Poller();
    TimeStamp poll(ChannelList& activeChannels, int timeoutMs);

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

    void assertInLoopThread() {
        _ownerLoop->assertInLoopThread();
    } 
};



#endif