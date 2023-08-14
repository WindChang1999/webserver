#include<gtest/gtest.h>
#include<net/InetAddress.hpp>
#include<string>

TEST(InetAddressTest, Constructor1) {
    InetAddress addr(8080);
    EXPECT_TRUE(addr.toString() == std::string("0.0.0.0:8080")) << addr.toString();
}

TEST(InetAddressTest, Constructor2) {
    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    const char* ip = "192.168.1.1";
    uint16_t port = 7890;
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sa.sin_addr);
    sa.sin_port = htons(port);
    InetAddress addr(sa);
    EXPECT_TRUE(addr.toString() == std::string("192.168.1.1:7890"));
}