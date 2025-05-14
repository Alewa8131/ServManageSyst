// Copyright 2025 Alewa8131
#pragma once
#include "../GameEntitiesLib/Server.h"
#include <string>

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
