#ifndef NET_INETADDRESS_HPP
#define NET_INETADDRESS_HPP

#include <arpa/inet.h>
#include <string>

class InetAddress {
public:
    explicit InetAddress(uint16_t port);
    explicit InetAddress(const struct sockaddr_in& addr)
        : _addr(addr) {}
    InetAddress(std::string ip, uint16_t port);
    std::string toString() const;
private:
    sockaddr_in _addr;
};

#endif