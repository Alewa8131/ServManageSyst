// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../GameEntitiesLib/Server.h"

class Advertisement {
 private:
    std::string _name;
    Server* _targetServer;
    double _cost;
    double _duration;
    std::string _status;

 public:
    void start();
    void stop();
};
