#include<gtest/gtest.h>
#include<utils/TimeStamp.hpp>
#include<string>

using namespace std;

TEST(TimeStampTest, CompareTime) {
    TimeStamp now0(TimeStamp::now());
    sleep(1);
    TimeStamp now1(TimeStamp::now());
    EXPECT_EQ(now0 < now1, true)
        << "now0: " << now0.toString() << endl
        << "now1: " << now1.toString() << endl;
}

TEST(TimeStampTest, TimeToString) {
    TimeStamp inv;
    EXPECT_STREQ(inv.toString().c_str(), "0.000000");
    EXPECT_STREQ(inv.toFormattedString().c_str(), 
        "19700101 00:00:00.000000") << inv.toFormattedString() << endl;
    EXPECT_STREQ(inv.toFormattedString(false).c_str(), 
        "19700101 00:00:00") << inv.toFormattedString(false) << endl;
}