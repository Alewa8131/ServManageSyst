// Copyright 2025 Alewa8131
#pragma once
#include <string>

class DateTime {
 private:
    int _day, _month, _year, _hour, _minute, _second;

 public:
    DateTime();
    DateTime(int d, int m, int y, int h, int min, int s);
    void set_current();
    std::string to_string() const;
    bool isPast() const;
    bool operator<(const DateTime& other) const;
};
