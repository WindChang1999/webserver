#include<poll.h>
#include<utils/TimeStamp.hpp>
#include<net/EventLoop.hpp>
#include<net/Channel.hpp>
#include<net/Poller.hpp>
#include<spdlog/spdlog.h>

Poller::Poller(EventLoop* loop): _ownerLoop(loop) {}

Poller::~Poller() {

}

TimeStamp Poller::poll(int timeoutMs, ChannelList& activeChannels) {
    int numEvents = ::poll(_pollfds.data(), _pollfds.size(), timeoutMs);
    TimeStamp now(TimeStamp::now());
    if (numEvents > 0) {
        spdlog::trace("{:d} events happened", numEvents);
        for (const auto& pollfd: _pollfds) {
            if (numEvents == 0) break;
            if (pollfd.revents > 0) {
                --numEvents;
                assert(_channels.count(pollfd.fd) > 0 
                    && "There is no corresponding fd");
                Channel* channel = _channels[pollfd.fd];
                channel->set_revents(pollfd.revents);
                activeChannels.push_back(channel);
            }
        }
    } else if (numEvents == 0){
        spdlog::trace("Poller::poll {:d}ms timeout", timeoutMs);
    } else {
        spdlog::critical("Poller::poll error");
    }
    return now;
}

void Poller::updateChannel(Channel* channel) {
    Poller::assertInLoopThread();

    int fd = channel->fd();
    int index = channel->index();
    short events = channel->events();
    if (index >= 0) {
        assert(_channels.count(fd) > 0 && "Should have exist");
        assert(_channels[fd] == channel);
        assert(_pollfds[index].fd == fd);
        _pollfds[index].events = events;
        _pollfds[index].revents = 0;
    } else {
        assert(_channels.count(fd) == 0 && "Shouldn't exist");
        _pollfds.emplace_back(fd, events, 0);
        index = static_cast<int>(_pollfds.size() - 1);
        channel->set_index(index);
        _channels[fd] = channel;
    }
}

void Poller::removeChannel(Channel* channel) {
    Poller::assertInLoopThread();

    int fd = channel->fd();
    int index = channel->index();
    assert(_channels.count(fd) > 0 && "Should have exist");
    assert(_channels[fd] == channel);
    _channels.erase(fd);

    if (index == _pollfds.size() - 1) {
        _pollfds.pop_back();
    } else {
        int fdOfChannelAtEnd = _pollfds.back().fd;
        std::swap(_pollfds[index], _pollfds.back());
        _channels[fdOfChannelAtEnd]->set_index(index);
        _pollfds.pop_back();
    }
}