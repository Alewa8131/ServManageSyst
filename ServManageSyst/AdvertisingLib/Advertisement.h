#pragma once
#include <Server.h>

class Advertisement {
private:
    std::string name;
    Server* targetServer;
    double cost;
    double duration;
    std::string status;
public:
    void start();
    void stop();
};