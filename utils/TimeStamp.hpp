#ifndef UTILS_TIMESTAMP_HPP
#define UTILS_TIMESTAMP_HPP

#include<string>

class TimeStamp
{
    friend bool operator<(TimeStamp lhs, TimeStamp rhs);
    friend bool operator==(TimeStamp lhs, TimeStamp rhs);
private:
    int64_t _microSecondsSinceEpoch;
    static constexpr int64_t microSecondsPerSecond = 1000 * 1000;
public:
    TimeStamp(): _microSecondsSinceEpoch(0) {}
    TimeStamp(int64_t microSecondsSinceEpoch)
        : _microSecondsSinceEpoch(microSecondsSinceEpoch) {}
    ~TimeStamp() {};

    std::string toString() const;
    std::string toFormattedString(bool showMicroseconds = true) const;

    static TimeStamp now(); 
};

inline bool operator<(TimeStamp lhs, TimeStamp rhs) {
    return lhs._microSecondsSinceEpoch < rhs._microSecondsSinceEpoch;
}

inline bool operator==(TimeStamp lhs, TimeStamp rhs) {
    return lhs._microSecondsSinceEpoch == rhs._microSecondsSinceEpoch;
}



#endif