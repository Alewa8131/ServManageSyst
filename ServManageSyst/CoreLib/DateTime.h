#pragma once
#include <string>

class DateTime {
private:
    int day, month, year, hour, minute;
public:
    DateTime();
    DateTime(int d, int m, int y, int h = 0, int min = 0);
    std::string toString() const;
    bool isPast() const;
    bool operator<(const DateTime& other) const;
};