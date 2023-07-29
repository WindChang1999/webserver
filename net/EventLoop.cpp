#include<net/EventLoop.hpp>
#include<thread>
#include<cassert>
#include<spdlog/spdlog.h>

thread_local EventLoop* loopInThisThread = nullptr;

void EventLoop::abortNotInLoopThread() {
    // FIXME: LOG_FATAL
    spdlog::critical("Start EventLoop in another thread!");
}

EventLoop::EventLoop():
    _looping(false),
    _tid(std::this_thread::get_id()) {
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
    // TODO: do something
}