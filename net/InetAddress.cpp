#include<net/InetAddress.hpp>
#include<string>
#include<sstream>
#include<cstring>

InetAddress::InetAddress(uint16_t port) {
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    in_addr_t ip = INADDR_ANY;
    _addr.sin_addr.s_addr = htonl(ip);
    _addr.sin_port = htons(port);
}

std::string InetAddress::toString() const {
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &_addr.sin_addr, buf, INET_ADDRSTRLEN);
    std::stringstream ss;
    uint16_t port = ntohs(_addr.sin_port);
    ss << buf << ":" << port;
    return ss.str();
}

InetAddress::InetAddress(std::string ip, uint16_t port) {
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr);
    _addr.sin_port = htons(port);
}