#ifndef NET_EVENTLOOP_HPP
#define NET_EVENTLOOP_HPP

#include<unistd.h>
#include<thread>
#include<memory>
#include<vector>

class Poller;
class Channel;

class EventLoop {
private:
    using ChannelList = std::vector<Channel*>;
    bool _looping;
    bool _quit;
    const std::thread::id _tid;
    void abortNotInLoopThread();
    std::unique_ptr<Poller> _poller;
    ChannelList _activeChannels;

    static const int kPollTimeMs = 10000;
public:
    EventLoop();
    ~EventLoop();
    
    void loop();
    void quit();  

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {
        return _tid == std::this_thread::get_id();
    }

    static EventLoop* getEventLoopOfCurrentThread();

    void updateChannel(Channel* channel);
};

#endif //NET_EVENTLOOP_HPP