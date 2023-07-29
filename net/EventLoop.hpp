#ifndef NET_EVENTLOOP_HPP
#define NET_EVENTLOOP_HPP

#include<unistd.h>
#include<thread>

class Channel;

class EventLoop {
private:
    bool _looping;
    const std::thread::id _tid;
    void abortNotInLoopThread();
    
public:
    EventLoop();
    ~EventLoop();
    
    void loop();

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const {
        // FIXME: should test _tid == current_tid
        return _tid == std::this_thread::get_id();
    }

    static EventLoop* getEventLoopOfCurrentThread();

    void updateChannel(Channel* channel);
};

#endif //NET_EVENTLOOP_HPP