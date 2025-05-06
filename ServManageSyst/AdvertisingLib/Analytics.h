// Copyright 2025 Alewa8131
#pragma once
#include <string>

class Analytics {

 private:
    std::string reportPeriod;
    double newPlayers;
    double activePercentage;
    double avgCheck;
    double profit;

 public:
    void calculate();
};
