#include<functional>

class EventLoop;

class Channel {
public:
    using EventCallBack = std::function<void()>;

private:
    EventCallBack _readCallBack, _writeCallBack, _errorCallBack;
    const int _fd;
    short _events;
    short _revents;
    int _index;
    EventLoop* _ownerloop;
    static const int kNoneEvent, kReadEvent, kWriteEvent;

    void update();

public: 
    Channel(EventLoop* loop, int fd);
    void handleEvent();
    void setReadCallBack(const EventCallBack& cb) {
        _readCallBack = cb;
    }
    void setWriteCallBack(const EventCallBack& cb) {
        _writeCallBack = cb;
    }
    void setErrorCallBack(const EventCallBack& cb) {
        _errorCallBack = cb;
    }
    int fd() const { return _fd; }
    int events() const { return _events; }
    void set_revents(int revent) { _revents = revent; } 
    int index() { return _index; }
    void set_index(int index) { _index = index; }
    
    void enableReading() { 
        _events |= kReadEvent;
        update(); 
    }
    void enableWriting() { 
        _events |= kReadEvent;
        update(); 
    }
    void disableWriting() {
        _events &= ~kWriteEvent;
        update();
    }
    void disableAll() {
        _events = kNoneEvent;
        update();
    }
};