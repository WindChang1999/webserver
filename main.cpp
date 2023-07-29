#include<iostream>
#include<string>
#include<net/EventLoop.hpp>

using namespace std;


int main(int argc, char *argv[]) {
    EventLoop loop;
    loop.loop();
    cout << "Hello World!" << endl;
    return 0;
}