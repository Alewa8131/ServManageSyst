// Copyright 2025 Alewa8131
#pragma once
#include <string>
#include "../CoreLib/DateTime.h"

class Privilege {
 private:
    std::string name;
    double price;
    DateTime purchaseDate;
    int durationDays;

 public:
    void setName(const std::string& name);
    void setPrice(double price);
    void printInfo() const;
};
