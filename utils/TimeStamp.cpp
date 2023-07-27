#include<utils/TimeStamp.hpp>
#include<sys/time.h>
#include<cinttypes>


static_assert(sizeof(TimeStamp) == sizeof(int64_t),
                "TimeStamp and int64_t should be the same size");

TimeStamp TimeStamp::now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    TimeStamp ts(tv.tv_sec * microSecondsPerSecond + tv.tv_usec);
    return ts;
}

std::string TimeStamp::toString() const{
    char buf[32] = {0};
    int64_t seconds = _microSecondsSinceEpoch / microSecondsPerSecond;
    int64_t microSeconds = _microSecondsSinceEpoch % microSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64, seconds, microSeconds);    
    return std::string(buf);
}

std::string TimeStamp::toFormattedString(bool showMicroSeconds) const {
    char buf[32] = {0};
    int64_t seconds = _microSecondsSinceEpoch / microSecondsPerSecond;
    time_t timer = static_cast<time_t>(seconds);
    struct tm tp;
    gmtime_r(&timer, &tp);
    if (showMicroSeconds) {
        int microSeconds = _microSecondsSinceEpoch % microSecondsPerSecond;
        snprintf(buf, sizeof(buf), "%04d%02d%02d %02d:%02d:%02d.%06d",
            tp.tm_year + 1900, tp.tm_mon + 1, tp.tm_mday, tp.tm_hour,
            tp.tm_min, tp.tm_sec, microSeconds);
    } else {
        snprintf(buf, sizeof(buf), "%04d%02d%02d %02d:%02d:%02d",
            tp.tm_year + 1900, tp.tm_mon + 1, tp.tm_mday, tp.tm_hour,
            tp.tm_min, tp.tm_sec);
    }
    return std::string(buf);
}