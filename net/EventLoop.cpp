#include<net/EventLoop.hpp>
#include<net/Poller.hpp>
#include<net/Channel.hpp>
#include<thread>
#include<cassert>
#include<memory>
#include<spdlog/spdlog.h>

thread_local EventLoop* loopInThisThread = nullptr;

void EventLoop::abortNotInLoopThread() {
    // FIXME: LOG_FATAL
    spdlog::critical("Start EventLoop in another thread!");
}

EventLoop::EventLoop():
    _looping(false),
    _tid(std::this_thread::get_id()),
    _poller(new Poller(this)) {
    if (loopInThisThread != nullptr) {
        // FIXME: LOG_FATAL
        spdlog::critical("This thread has another EventLoop!");
    } else {
        loopInThisThread = this;
    } 
}

EventLoop::~EventLoop() {
    // assert(!_looping && "EventLoop still looping!");
    loopInThisThread = nullptr;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return loopInThisThread;
}

void EventLoop::loop() {
    assert(!_looping && "EventLoop has been started!");
    assert(isInLoopThread() && "Start EventLoop from another thread!");
    _looping = true;
    _quit = false;
    while (!_quit) {
        _activeChannels.clear();
        _poller->poll(_activeChannels, kPollTimeMs);
        for (auto channel: _activeChannels) {
            channel->handleEvent();
        }
    }
    // FIXME: std::thread::id is not formattable
    // spdlog::trace("EventLoop {} stop", _tid);
    _looping = false;
}

void EventLoop::quit() { 
    _quit = true;
    // FIXME: Should wake up
}


void EventLoop::updateChannel(Channel* channel) {
    assert(_poller != nullptr);
    _poller->updateChannel(channel);
}