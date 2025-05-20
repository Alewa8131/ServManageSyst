// Copyright 2025 Alewa8131
#include "DateTime.h"

#include <iostream>

DateTime::DateTime() {
    set_current();
}
DateTime::DateTime(int d, int m, int y, int h = 0, int min = 0, int s = 0) {
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
    if (d > 31 && ((m == 1) || (m == 3) || (m == 5)
        || (m == 7) || (m == 8) || (m == 10) || (m == 12))) {
        m += d / 31;
        d = d % 31;
    } else if (d > 30 && ((m == 4) || (m == 6) || (m == 9) || (m == 11))) {
        m += d / 30;
        d = d % 30;
    } else if (d > 29 && m == 2 && y % 4 == 0) {
        m += d / 29;
        d = d % 29;
    } else if (d > 28 && m == 2) {
        m += d / 28;
        d = d % 28;
    }
    if (m >= 13) {
        y += m / 12;
        m = m % 12;
    }
    _day = d; _month = m; _year = y; _hour = h; _minute = min; _second = s;
}

void DateTime::set_current() {
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

std::string DateTime::to_string() const {
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

bool DateTime::isPast() const {
    std::time_t now = std::time(nullptr);
    std::tm current;
    localtime_s(&current, &now);

    if (_year < current.tm_year + 1900) return true;
    if (_year > current.tm_year + 1900) return false;

    if (_month < current.tm_mon + 1) return true;
    if (_month > current.tm_mon + 1) return false;

    if (_day < current.tm_mday) return true;
    if (_day > current.tm_mday) return false;

    if (_hour < current.tm_hour) return true;
    if (_hour > current.tm_hour) return false;

    if (_minute < current.tm_min) return true;
    if (_minute > current.tm_min) return false;

    return _second < current.tm_sec;
}

bool DateTime::operator<(const DateTime& other) const {
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
    } else return false;
}
