// Copyright 2025 Alewa8131
#pragma once
#include <string>

class Analytics {
 private:
    std::string _reportPeriod;
    double _newPlayers;
    double _activePercentage;
    double _avgCheck;
    double _profit;

 public:
    void calculate();
};
