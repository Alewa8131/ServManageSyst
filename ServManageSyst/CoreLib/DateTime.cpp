// Copyright 2025 Alewa8131
#include "DateTime.h"
#include <string>
#include <iostream>

Core::DateTime::DateTime() {
    set_current();
}
Core::DateTime::DateTime(int d, int m, int y, int h = 0, int min = 0, int s = 0) {
    if (d < 0 || m < 0 || y < 0 || h < 0 || min < 0) {
        throw std::logic_error("The date cannot be negative!");
    }
    if (s >= 60) {
        min += s / 60;
        s = s % 60;
    }
    if (min >= 60) {
        h += min / 60;
        min = min % 60;
    }
    if (h >= 24) {
        d += h / 24;
        h = h % 24;
    }
    while (true) {
        int DinM = days_in_month(m, y);
        if (d <= DinM) break;
        d -= DinM;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }
    _day = d; _month = m; _year = y; _hour = h; _minute = min; _second = s;
}
Core::DateTime::DateTime(std::string date) {
    *this = from_string(date);
}

void Core::DateTime::add_days(int days) {
    _day += days;

    while (true) {
        int currentDays = days_in_month(_month, _year);
        if (_day <= currentDays) break;

        _day -= currentDays;
        _month++;

        if (_month > 12) {
            _month = 1;
            _year++;
        }
    }
}
void Core::DateTime::set_current() {
    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
    localtime_s(&timeInfo, &now);

    _day = timeInfo.tm_mday;
    _month = timeInfo.tm_mon + 1;
    _year = timeInfo.tm_year + 1900;
    _hour = timeInfo.tm_hour;
    _minute = timeInfo.tm_min;
    _second = timeInfo.tm_sec;
}

std::string Core::DateTime::to_string() const {
    std::string result;

    if (_day < 10) result += "0";
    result += std::to_string(_day) + ".";

    if (_month < 10) result += "0";
    result += std::to_string(_month) + ".";

    result += std::to_string(_year) + " ";

    if (_hour < 10) result += "0";
    result += std::to_string(_hour) + ":";

    if (_minute < 10) result += "0";
    result += std::to_string(_minute) + ":";

    if (_second < 10) result += "0";
    result += std::to_string(_second);

    return result;
}
Core::DateTime Core::DateTime::from_string(const std::string& str) {
    int d = 0, m = 0, y = 0;
    int h = 0, min = 0, s = 0;

    if (str.find('.') != std::string::npos) {
        // dd.mm.yyyy or dd.mm.yyyy hh:mm:ss
        if (str.length() < 10)
            throw std::invalid_argument("Too short for date format: " + str);
        d = (str[0] - '0') * 10 + (str[1] - '0');
        m = (str[3] - '0') * 10 + (str[4] - '0');
        y = (str[6] - '0') * 1000 + (str[7] - '0') * 100
            + (str[8] - '0') * 10 + (str[9] - '0');

        if (str.length() > 11 && str[10] == ' ') {
            h = (str[11] - '0') * 10 + (str[12] - '0');
            min = (str[14] - '0') * 10 + (str[15] - '0');
            s = (str[17] - '0') * 10 + (str[18] - '0');
        }
    } else if (str.find(':') != std::string::npos) {
        // hh:mm:ss
        if (str.length() != 8)
            throw std::invalid_argument("Invalid time format: " + str);
        h = (str[0] - '0') * 10 + (str[1] - '0');
        min = (str[3] - '0') * 10 + (str[4] - '0');
        s = (str[6] - '0') * 10 + (str[7] - '0');
    } else {
        throw std::invalid_argument("Unrecognized DateTime format: " + str);
    }

    return DateTime(d, m, y, h, min, s);
}


bool Core::DateTime::isPast() const {
    DateTime now;
    return *this < now;
}

bool Core::DateTime::operator<(const DateTime& other) const {
    if (this->_year < other._year) {
        return true;
    } else if (this->_month < other._month) {
        return true;
    } else if (this->_day < other._day) {
        return true;
    } else if (this->_hour < other._hour) {
        return true;
    } else if (this->_minute < other._minute) {
        return true;
    } else if (this->_second < other._second) {
        return true;
    } else {
        return false;
    }
}

bool Core::DateTime::is_leap(int year) const {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
int Core::DateTime::days_in_month(int month, int year) const {
    switch (month) {
    case 1: case 3: case 5: case 7:
    case 8: case 10: case 12: return 31;

    case 4: case 6: case 9: case 11: return 30;

    case 2: return is_leap(year) ? 29 : 28;
    default: return 0;
    }
}

int Core::DateTime::days_since_epoch() const {
    int days = _day;

    for (int m = 1; m < _month; ++m)
        days += days_in_month(m, _year);

    for (int y = 0; y < _year; ++y)
        days += is_leap(y) ? 366 : 365;

    return days;
}
int Core::DateTime::difference_in_days(const DateTime& other) const {
    return other.days_since_epoch() - this->days_since_epoch();
}
