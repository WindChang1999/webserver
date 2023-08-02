#include<poll.h>
#include<net/EventLoop.hpp>
#include<net/Channel.hpp>
#include<spdlog/spdlog.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd):
    _ownerloop(loop),
    _fd(fd),
    _events(kNoneEvent),
    _revents(kNoneEvent),
    _index(-1) {}


void Channel::update() {
    assert(_ownerloop != nullptr);
    _ownerloop->updateChannel(this);
}

void Channel::handleEvent() {
    if (_revents & POLLNVAL) {
        spdlog::warn("Invalid polling request");
    }

    if (_revents & (POLLERR | POLLNVAL)) {
        if (_errorCallBack) _errorCallBack();
    }

    if (_revents & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (_readCallBack) _readCallBack();
    }

    if (_revents & POLLOUT) {
        if (_writeCallBack) _writeCallBack();
    }
}