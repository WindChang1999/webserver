#include<poll.h>
#include<net/Poller.hpp>
#include<net/Channel.hpp>
#include<spdlog/spdlog.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(Poller* poller, int fd):
    _poller(poller),
    _fd(fd),
    _events(kNoneEvent),
    _revents(kNoneEvent),
    _index(-1) {}


void Channel::update() {
    assert(_poller != nullptr);
    _poller->updateChannel(this);
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